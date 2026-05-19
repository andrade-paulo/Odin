#include "adapter_sx1276.hpp"
#include "esp_log.h"
#include "rom/ets_sys.h" // for ets_delay_us

static const char* TAG = "SX1276_PHY";

// SX1276 Register Map (Subset)
#define REG_FIFO           0x00
#define REG_OP_MODE        0x01
#define REG_FRF_MSB        0x06
#define REG_FRF_MID        0x07
#define REG_FRF_LSB        0x08
#define REG_PA_CONFIG      0x09
#define REG_LNA            0x0C
#define REG_IRQ_FLAGS      0x12
#define REG_MODEM_CONFIG_1 0x1D
#define REG_MODEM_CONFIG_2 0x1E
#define REG_PREAMBLE_MSB   0x20
#define REG_PREAMBLE_LSB   0x21
#define REG_FIFO_ADDR_PTR  0x0D
#define REG_FIFO_TX_BASE   0x0E
#define REG_FIFO_RX_BASE   0x0F
#define REG_PAYLOAD_LENGTH 0x22
#define REG_MODEM_CONFIG_3 0x26
#define REG_DIO_MAPPING_1  0x40
#define REG_VERSION        0x42
#define REG_SYNC_WORD      0x39

#define MODE_LONG_RANGE_MODE 0x80
#define MODE_SLEEP           0x00
#define MODE_STDBY           0x01
#define MODE_TX              0x03
#define IRQ_TX_DONE_MASK     0x08

namespace infra {

SX1276_Adapter::SX1276_Adapter(spi_host_device_t host, int cs, int reset, int dio0)
    : spi_host(host), pin_cs((gpio_num_t)cs), pin_reset((gpio_num_t)reset), pin_dio0((gpio_num_t)dio0) {
    
    // Create the binary semaphore. It starts "empty" (0).
    tx_done_sem = xSemaphoreCreateBinary();
}

void IRAM_ATTR SX1276_Adapter::dio0_isr_handler(void* arg) {
    auto* adapter = static_cast<SX1276_Adapter*>(arg);
    BaseType_t high_task_wakeup = pdFALSE;
    
    // Give the semaphore to wake up the LoRa TX Task
    xSemaphoreGiveFromISR(adapter->tx_done_sem, &high_task_wakeup);
    
    if (high_task_wakeup == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

bool SX1276_Adapter::initialize() {
    esp_err_t ret;

    // 1. Attach to the existing SPI bus
    spi_device_interface_config_t devcfg = {};
    devcfg.clock_speed_hz = 5000000; // 5 MHz is plenty for LoRa SPI
    devcfg.mode = 0;
    devcfg.spics_io_num = pin_cs;
    devcfg.queue_size = 3;

    ret = spi_bus_add_device(spi_host, &devcfg, &spi_device);
    if (ret != ESP_OK) return false;

    // 2. Hardware Reset the SX1276
    gpio_set_direction(pin_reset, GPIO_MODE_OUTPUT);
    gpio_set_level(pin_reset, 0);
    ets_delay_us(1000);
    gpio_set_level(pin_reset, 1);
    vTaskDelay(pdMS_TO_TICKS(10));

    // 3. Configure DIO0 Interrupt (Rising Edge for TX_DONE)
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.pin_bit_mask = (1ULL << pin_dio0);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    ret = gpio_config(&io_conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure DIO0 pin: %s", esp_err_to_name(ret));
        return false;
    }
    
    ret = gpio_install_isr_service(0);
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Failed to install GPIO ISR service: %s", esp_err_to_name(ret));
        return false;
    }

    ret = gpio_isr_handler_add(pin_dio0, dio0_isr_handler, (void*)this);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add DIO0 ISR handler: %s", esp_err_to_name(ret));
        return false;
    }

    // 4. Put into LoRa Sleep, then Standby to configure
    write_register(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
    vTaskDelay(pdMS_TO_TICKS(10));

    // Verify chip presence over SPI (SX1276 version is expected to be 0x12).
    uint8_t version = read_register(REG_VERSION);
    if (version != 0x12) {
        ESP_LOGE(TAG, "SX1276 version mismatch: 0x%02X (expected 0x12)", version);
        return false;
    }

    write_register(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);

    // Configure frequency to 915 MHz by default (FRF = Freq * 2^19 / 32MHz).
    const uint64_t freq_hz = 915000000ULL;
    const uint32_t frf = (uint32_t)((freq_hz << 19) / 32000000ULL);
    write_register(REG_FRF_MSB, (uint8_t)(frf >> 16));
    write_register(REG_FRF_MID, (uint8_t)(frf >> 8));
    write_register(REG_FRF_LSB, (uint8_t)(frf));

    // LoRa modem config: BW=125kHz, CR=4/5, explicit header, SF=7, CRC on, AGC auto.
    write_register(REG_MODEM_CONFIG_1, 0x72);
    write_register(REG_MODEM_CONFIG_2, 0x74);
    write_register(REG_MODEM_CONFIG_3, 0x04);

    // Reasonable defaults for packet framing and RF output.
    write_register(REG_PREAMBLE_MSB, 0x00);
    write_register(REG_PREAMBLE_LSB, 0x08);
    write_register(REG_SYNC_WORD, 0x34);
    write_register(REG_PA_CONFIG, 0x8F);
    write_register(REG_LNA, 0x23);
    write_register(REG_FIFO_TX_BASE, 0x00);
    write_register(REG_FIFO_RX_BASE, 0x00);
    write_register(REG_IRQ_FLAGS, 0xFF);
    
    // Map DIO0 to TX_DONE.
    write_register(REG_DIO_MAPPING_1, 0x40); 

    ESP_LOGI(TAG, "SX1276 PHY Initialized");

    this->initialized_flag = true;

    return true;
}

void SX1276_Adapter::write_register(uint8_t reg, uint8_t data) {
    spi_transaction_t t = {};
    t.length = 16; 
    t.flags = SPI_TRANS_USE_TXDATA;
    t.tx_data[0] = reg | 0x80; // MSB must be 1 for Write
    t.tx_data[1] = data;
    spi_device_polling_transmit(spi_device, &t);
}

uint8_t SX1276_Adapter::read_register(uint8_t reg) {
    spi_transaction_t t = {};
    t.length = 16;
    t.rxlength = 16;
    t.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA;
    t.tx_data[0] = reg & 0x7F; // MSB must be 0 for Read
    t.tx_data[1] = 0x00;        // Dummy byte clocks out register value

    esp_err_t ret = spi_device_polling_transmit(spi_device, &t);
    if (ret != ESP_OK) {
        return 0;
    }

    return t.rx_data[1];
}

void SX1276_Adapter::write_fifo(const uint8_t* data, size_t length) {
    spi_transaction_t t = {};
    t.length = 8 * (length + 1); // 1 byte address + data
    uint8_t* tx_buf = (uint8_t*)heap_caps_malloc(length + 1, MALLOC_CAP_DMA);
    
    tx_buf[0] = REG_FIFO | 0x80;
    memcpy(&tx_buf[1], data, length);
    t.tx_buffer = tx_buf;
    
    spi_device_polling_transmit(spi_device, &t);
    free(tx_buf);
}

bool SX1276_Adapter::transmit_packet_blocking(const uint8_t* data, size_t length) {
    if (length > 255) return false; // Hardware limit

    // Drain any stale semaphore token from a previous ISR.
    xSemaphoreTake(tx_done_sem, 0);

    // 1. Reset FIFO pointers
    write_register(REG_FIFO_TX_BASE, 0x00);
    write_register(REG_FIFO_ADDR_PTR, 0x00);

    // 1.1 Program payload length and clear stale IRQ flags.
    write_register(REG_PAYLOAD_LENGTH, (uint8_t)length);
    write_register(REG_IRQ_FLAGS, 0xFF);

    // 2. Load Payload into Radio via SPI
    write_fifo(data, length);

    // 3. Command TX Mode (Radio starts radiating here)
    write_register(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

    // 4. Wait for TX_DONE using ISR semaphore first, with register polling fallback.
    const TickType_t total_timeout = pdMS_TO_TICKS(2000);
    TickType_t poll_step = pdMS_TO_TICKS(20);
    if (poll_step == 0) {
        poll_step = 1;
    }
    TickType_t waited = 0;

    while (waited < total_timeout) {
        if (xSemaphoreTake(tx_done_sem, poll_step) == pdTRUE) {
            write_register(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
            write_register(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
            ESP_LOGI(TAG, "TX Done (%u bytes)", (unsigned)length);
            return true;
        }

        if ((read_register(REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) != 0) {
            write_register(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
            write_register(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
            ESP_LOGI(TAG, "TX Done (%u bytes)", (unsigned)length);
            return true;
        }

        waited += poll_step;
    }

    ESP_LOGE(TAG, "TX Timeout! Radio lockup detected.");
    // Fallback: clear IRQ and force radio back to standby.
    write_register(REG_IRQ_FLAGS, 0xFF);
    write_register(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
    return false;
}

}
