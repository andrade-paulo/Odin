#include "adapter_sx1276.hpp"
#include "esp_log.h"
#include "rom/ets_sys.h" // for ets_delay_us

static const char* TAG = "SX1276_PHY";

// SX1276 Register Map (Subset)
#define REG_FIFO           0x00
#define REG_OP_MODE        0x01
#define REG_IRQ_FLAGS      0x12
#define REG_FIFO_ADDR_PTR  0x0D
#define REG_FIFO_TX_BASE   0x0E
#define REG_DIO_MAPPING_1  0x40

#define MODE_LONG_RANGE_MODE 0x80
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
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_config(&io_conf);
    
    gpio_install_isr_service(0);
    gpio_isr_handler_add(pin_dio0, dio0_isr_handler, (void*)this);

    // 4. Put into LoRa Sleep, then Standby to configure
    write_register(REG_OP_MODE, MODE_LONG_RANGE_MODE | 0x00); // Sleep
    vTaskDelay(pdMS_TO_TICKS(10));
    write_register(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);

    // (Omitted for brevity: Configuring Frequency, Spreading Factor, Bandwidth, PA Output Power)
    
    // Map DIO0 to TX_DONE
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

    // 1. Reset FIFO pointers
    write_register(REG_FIFO_TX_BASE, 0x00);
    write_register(REG_FIFO_ADDR_PTR, 0x00);

    // 2. Load Payload into Radio via SPI
    write_fifo(data, length);

    // 3. Command TX Mode (Radio starts radiating here)
    write_register(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

    // 4. Yield CPU until DIO0 ISR fires (TX Done)
    // Timeout set to 2 seconds as a fail-safe in case of RF lockup
    if (xSemaphoreTake(tx_done_sem, pdMS_TO_TICKS(2000)) == pdTRUE) {
        // Clear the IRQ register
        write_register(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK); 
        return true;
    } else {
        ESP_LOGE(TAG, "TX Timeout! Radio lockup detected.");
        // Fallback: Force radio back to standby
        write_register(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
        return false;
    }
}

}
