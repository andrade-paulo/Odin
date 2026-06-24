#include "lora_sx1276.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "rom/ets_sys.h"

static const char* TAG = "LORA_DRV";

// Helper para ler registradores do SX1276
static void lora_read_reg(spi_device_handle_t spi, uint8_t reg, uint8_t* val) {
    uint8_t tx[2] = { reg & 0x7F, 0x00 };
    uint8_t rx[2] = { 0 };
    spi_transaction_t t = { .length = 16, .tx_buffer = tx, .rx_buffer = rx };
    spi_device_transmit(spi, &t);
    *val = rx[1];
}

static void IRAM_ATTR lora_dio0_isr(void* arg) {
    lora_ctx_t* ctx = (lora_ctx_t*) arg;
    BaseType_t high_task_awoken = pdFALSE;
    vTaskNotifyGiveFromISR(ctx->isr_task, &high_task_awoken);
    if (high_task_awoken) portYIELD_FROM_ISR();
}

static void lora_isr_handler_task(void* arg) {
    lora_ctx_t* ctx = (lora_ctx_t*) arg;
    lora_rx_packet_t packet;
    uint8_t irq_flags, fifo_rx_current, length;

    ESP_LOGI("LORA_DRV", "Modo de Interrupção de Hardware (DIO0) Iniciado.");

    while(1) {
        // Bloqueia permanentemente, sem consumir CPU, até que o pino físico pulse
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        lora_read_reg(ctx->spi_handle, 0x12, &irq_flags); // REG_IRQ_FLAGS
        
        if (irq_flags & 0x40) { // Verifica a máscara interna para RxDone
            
            lora_read_reg(ctx->spi_handle, 0x10, &fifo_rx_current); // REG_FIFO_RX_CURRENT_ADDR
            lora_read_reg(ctx->spi_handle, 0x13, &length);          // REG_RX_NB_BYTES

            uint8_t tx_ptr[2] = { 0x0D | 0x80, fifo_rx_current }; // REG_FIFO_ADDR_PTR
            spi_transaction_t t_ptr = { .length = 16, .tx_buffer = tx_ptr };
            spi_device_transmit(ctx->spi_handle, &t_ptr);

            packet.length = length;
            uint8_t tx_buf[LORA_MAX_PAYLOAD + 1];
            uint8_t rx_buf[LORA_MAX_PAYLOAD + 1];
            tx_buf[0] = 0x00 & 0x7F; // REG_FIFO Read

            spi_transaction_t t_fifo = {
                .length = (length + 1) * 8,
                .tx_buffer = tx_buf,
                .rx_buffer = rx_buf
            };
            spi_device_transmit(ctx->spi_handle, &t_fifo);
            
            memcpy(packet.payload, &rx_buf[1], length);

            // Limpa flags (Write 1 to clear)
            uint8_t tx_clr[2] = { 0x12 | 0x80, 0xFF };
            spi_transaction_t t_clr = { .length = 16, .tx_buffer = tx_clr };
            spi_device_transmit(ctx->spi_handle, &t_clr);

            xQueueSendToBack(ctx->rx_queue, &packet, 0);
        }
    }
}

esp_err_t lora_init(lora_ctx_t* ctx) {
    spi_bus_config_t buscfg = {
        .miso_io_num = 19, .mosi_io_num = 23, .sclk_io_num = 18,
        .quadwp_io_num = -1, .quadhd_io_num = -1, .max_transfer_sz = 260
    };
    spi_bus_initialize(SPI3_HOST, &buscfg, SPI_DMA_CH_AUTO);

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 8000000, .mode = 0,
        .spics_io_num = ctx->cs_pin, .queue_size = 7
    };
    spi_bus_add_device(SPI3_HOST, &devcfg, &ctx->spi_handle);

    gpio_set_direction(ctx->rst_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(ctx->rst_pin, 0);
    vTaskDelay(pdMS_TO_TICKS(1));
    gpio_set_level(ctx->rst_pin, 1);
    vTaskDelay(pdMS_TO_TICKS(10));

    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_POSEDGE, .pin_bit_mask = (1ULL << ctx->irq_pin),
        .mode = GPIO_MODE_INPUT, .pull_down_en = GPIO_PULLDOWN_DISABLE
    };
    gpio_config(&io_conf);

    xTaskCreatePinnedToCore(lora_isr_handler_task, "lora_isr", 4096, ctx, configMAX_PRIORITIES - 1, &ctx->isr_task, 1);
    
    gpio_install_isr_service(0);
    gpio_isr_handler_add(ctx->irq_pin, lora_dio0_isr, (void*) ctx);

    uint8_t version;
    lora_read_reg(ctx->spi_handle, 0x42, &version); // REG_VERSION
    if (version != 0x12) {
        ESP_LOGE(TAG, "FALHA CRITICA: SX1276 não encontrado no barramento SPI. Lido: 0x%02X", version);
        // Não continua se o hardware estiver desconectado
        return ESP_FAIL; 
    }
    ESP_LOGI(TAG, "SX1276 detectado (Versão 0x%02X)", version);

    // 1. Força SLEEP absoluto (0x00)
    uint8_t tx_sleep[2] = { 0x01 | 0x80, 0x00 };
    spi_transaction_t t_sleep = { .length = 16, .tx_buffer = tx_sleep };
    spi_device_transmit(ctx->spi_handle, &t_sleep);
    vTaskDelay(pdMS_TO_TICKS(10));

    // 2. Entra em LoRa + SLEEP (0x80)
    uint8_t tx_lora_sleep[2] = { 0x01 | 0x80, 0x80 };
    spi_transaction_t t_lsleep = { .length = 16, .tx_buffer = tx_lora_sleep };
    spi_device_transmit(ctx->spi_handle, &t_lsleep);
    vTaskDelay(pdMS_TO_TICKS(10));

    // 3. Entra em LoRa + STANDBY (0x81) para parametrizar
    uint8_t tx_stby[2] = { 0x01 | 0x80, 0x81 };
    spi_transaction_t t_stby = { .length = 16, .tx_buffer = tx_stby };
    spi_device_transmit(ctx->spi_handle, &t_stby);
    vTaskDelay(pdMS_TO_TICKS(10));

    // 4. Frequência 915 MHz (0xE4C000)
    uint8_t tx_frf[4] = { 0x06 | 0x80, 0xE4, 0xC0, 0x00 };
    spi_transaction_t t_frf = { .length = 32, .tx_buffer = tx_frf };
    spi_device_transmit(ctx->spi_handle, &t_frf);

    // 5. Mapeia DIO0 para RxDone (00)
    uint8_t tx_dio[2] = { 0x40 | 0x80, 0x00 }; 
    spi_transaction_t t_dio = { .length = 16, .tx_buffer = tx_dio };
    spi_device_transmit(ctx->spi_handle, &t_dio);

    // 6. LNA Gain (Máximo)
    uint8_t tx_lna[2] = { 0x0C | 0x80, 0x23 }; 
    spi_transaction_t t_lna = { .length = 16, .tx_buffer = tx_lna };
    spi_device_transmit(ctx->spi_handle, &t_lna);

    // 7. Entra em RX Continuous (0x85)
    uint8_t tx_rx[2] = { 0x01 | 0x80, 0x85 }; 
    spi_transaction_t t_rx = { .length = 16, .tx_buffer = tx_rx };
    spi_device_transmit(ctx->spi_handle, &t_rx);

    ESP_LOGI(TAG, "LoRa RX Inicializado em 915MHz. Aguardando interrupções.");
    return ESP_OK;
}