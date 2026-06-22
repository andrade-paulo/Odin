#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/spi_master.h"
#include "driver/i2c.h"

// Camada de Domínio
#include "telemetry_orchestrator.hpp"

// Camada de Adapters e Infraestrutura
#include "i2c_bus_manager.hpp"
#include "lora_task.hpp"
#include "orchestrator_task.hpp"
#include "imu_task.hpp"
#include "barometer_task.hpp"
#include "gps_task.hpp"
// #include "consumer_uart_hil_task.hpp" // Opcional: Descomente se quiser espelhar no PC

static const char* TAG = "MAIN_SYSTEM";

// Pinagem LoRa (SPI2 / VSPI)
#define LORA_SCK_PIN  18
#define LORA_MISO_PIN 19
#define LORA_MOSI_PIN 23
#define LORA_CS_PIN   5
#define LORA_RST_PIN  14
#define LORA_DIO0_PIN 26

// Pinagem Sensores I2C
#define I2C_SDA_PIN   21
#define I2C_SCL_PIN   22

// Pinagem GPS UART
#define GPS_UART_NUM UART_NUM_1
#define GPS_TX_PIN    17
#define GPS_RX_PIN    16

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "=== Iniciando Sistema de Telemetria Odin (Aircraft Crow) ===");

    spi_bus_config_t spi_cfg = {};
    spi_cfg.miso_io_num = LORA_MISO_PIN;
    spi_cfg.mosi_io_num = LORA_MOSI_PIN;
    spi_cfg.sclk_io_num = LORA_SCK_PIN;
    spi_cfg.quadwp_io_num = -1;
    spi_cfg.quadhd_io_num = -1;
    spi_cfg.max_transfer_sz = 4096;
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &spi_cfg, SPI_DMA_CH_AUTO));

    auto* i2cBus = new I2cBusManager(I2C_NUM_0, I2C_SDA_PIN, I2C_SCL_PIN, 400000);
    if (!i2cBus->init()) {
        ESP_LOGE(TAG, "Falha catastrofica no I2C. Abortando inicializacao de sensores.");
        // Estado de erro
    }

    auto* loraTask = new LoRaTask(SPI2_HOST, LORA_CS_PIN, LORA_RST_PIN, LORA_DIO0_PIN, 1, 1);
    
    auto* core = new TelemetryOrchestrator(loraTask, nullptr, nullptr, nullptr);

    auto* orchestratorTask = new OrchestratorTask(core);

    auto* imuTask = new ImuTask(orchestratorTask, i2cBus);
    auto* baroTask = new BarometerTask(orchestratorTask, i2cBus);
    auto* gpsTask = new GpsTask(orchestratorTask, GPS_UART_NUM, GPS_TX_PIN, GPS_RX_PIN);

    ESP_LOGI(TAG, "Executando Boot Sequence das Tasks...");

    if (!loraTask->start()) ESP_LOGE(TAG, "Erro Crítico: LoRa Task");
    if (!orchestratorTask->start()) ESP_LOGE(TAG, "Erro Crítico: Orchestrator Task");
    
    // Tempo de acomodação para o rádio e orquestrador entrarem em estado Blocked
    vTaskDelay(pdMS_TO_TICKS(50));

    if (!imuTask->start()) ESP_LOGE(TAG, "Erro: IMU Task");
    if (!baroTask->start()) ESP_LOGE(TAG, "Erro: Barometer Task");
    if (!gpsTask->start()) ESP_LOGE(TAG, "Erro: GPS Task");

    ESP_LOGI(TAG, "=== Sistema Odin Operacional e Transmitindo Voo ===");
    
    // Essa thread principal pode encerrar para poupar RAM
}