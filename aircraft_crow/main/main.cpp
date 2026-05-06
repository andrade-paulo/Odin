#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/stream_buffer.h"
#include "esp_log.h"
#include "nvs_flash.h"

// Infrastructure Adapters
#include "adapter_mpu6050.hpp"
#include "adapter_sd_spi.hpp"
#include "adapter_sx1276.hpp"
#include "adapter_mavlink_radio.hpp"
#include "adapter_rc_receiver.hpp"
#include "adapter_buzzer.hpp"

// Core Domain
#include "telemetry_processor.hpp"
#include "telemetry_scheduler.hpp"
#include "task_params.hpp"

// Board Configuration (PINS)
#include "board_pinout_config.hpp"
#include <driver/sdspi_host.h>

static const char* TAG = "WIRING_LAYER";

static bool initialize_i2c_bus() {
    i2c_config_t config = {};
    config.mode = I2C_MODE_MASTER;
    config.sda_io_num = BoardConfig::I2C_SensorBus::SDA;
    config.scl_io_num = BoardConfig::I2C_SensorBus::SCL;
    config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    config.master.clk_speed = BoardConfig::I2C_SensorBus::FREQ;
    config.clk_flags = 0;

    esp_err_t ret = i2c_param_config(BoardConfig::I2C_SensorBus::PORT, &config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure I2C bus: %s", esp_err_to_name(ret));
        return false;
    }

    ret = i2c_driver_install(BoardConfig::I2C_SensorBus::PORT, config.mode, 0, 0, 0);
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Failed to install I2C driver: %s", esp_err_to_name(ret));
        return false;
    }

    return true;
}

static bool initialize_spi_bus() {
    spi_bus_config_t bus_cfg = {};
    bus_cfg.mosi_io_num = BoardConfig::SPI_Shared::MOSI;
    bus_cfg.miso_io_num = BoardConfig::SPI_Shared::MISO;
    bus_cfg.sclk_io_num = BoardConfig::SPI_Shared::CLK;
    bus_cfg.quadwp_io_num = -1;
    bus_cfg.quadhd_io_num = -1;
    bus_cfg.max_transfer_sz = 4000;

    esp_err_t ret = spi_bus_initialize(BoardConfig::SPI_Shared::HOST, &bus_cfg, 2);
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Failed to initialize SPI bus: %s", esp_err_to_name(ret));
        return false;
    }

    return true;
}

// --- Global IPC Variables ---
QueueHandle_t sd_data_queue = nullptr;
QueueHandle_t buzzer_queue = nullptr;
StreamBufferHandle_t lora_tx_stream = nullptr;

extern void vTaskAcquisition(void *pvParameters);
extern void vTaskSDLogger(void *pvParameters);
extern void vTaskLoRaTX(void *pvParameters);
extern void vTaskBuzzer(void *pvParameters);
extern void vTaskPWMLogger(void *pvParameters);

extern "C" void app_main() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    ESP_LOGI(TAG, "Booting UAV Telemetry Node...");

    sd_data_queue = xQueueCreate(50, sizeof(core::FlightDataDTO));
    buzzer_queue = xQueueCreate(10, sizeof(infra::BuzzerCommand));
    lora_tx_stream = xStreamBufferCreate(512, 1);

    if (!sd_data_queue || !lora_tx_stream) {
        ESP_LOGE(TAG, "Failed to allocate IPC memory in FreeRTOS heap.");
        return;
    }

    using namespace BoardConfig;

    bool i2c_ready = initialize_i2c_bus();
    bool spi_ready = initialize_spi_bus();

    // A. MPU6050 IMU
    auto* imu_adapter = new infra::MPU6050_Adapter(I2C_SensorBus::PORT, 0x68);
    bool imu_ready = i2c_ready && imu_adapter->initialize();
    bool imu_healthy = imu_ready && imu_adapter->is_healthy();

    // B. SD Card (Using Shared SPI)
    auto* sd_adapter = new infra::SDCardSPIAdapter(
        SPI_Shared::MOSI,  
        SPI_Shared::MISO, 
        SPI_Shared::CLK, 
        SPI_SDCard::CS, 
        "/sdcard"
    );

    bool sd_ready = sd_adapter->mount();

    // C. LoRa SX1276 (Using the exact same Shared SPI Host)
    auto* lora_adapter = new infra::SX1276_Adapter(
        SPI_Shared::HOST, 
        SPI_LoRa::CS, 
        SPI_LoRa::RST, 
        SPI_LoRa::DIO0
    );

    bool lora_ready = lora_adapter->initialize();

    if (!lora_ready) {
        ESP_LOGE(TAG, "LoRa init failed.");
    }

    // D. MAVLink Protocol Wrapper
    auto* mavlink_radio = new infra::MavlinkRadioAdapter(lora_tx_stream, 1, 1);

    // E. RC Receiver Switch
    auto* rc_adapter = new infra::RCReceiverAdapter(RC_Input::PWM_RX);
    rc_adapter->initialize();

    // F. Buzzer feedback
    auto* buzzer_adapter = new infra::BuzzerAdapter(Status::BUZZER);
    buzzer_adapter->initialize();
    
    // Start the task immediately so it can listen for boot success/errors
    xTaskCreatePinnedToCore(vTaskBuzzer, "BuzzerTask", 2048, (void*)buzzer_adapter, 2, NULL, 0);

    // Dependency Injection
    auto* telemetry_scheduler = new core::TelemetryScheduler<infra::MavlinkRadioAdapter>(*mavlink_radio);
    
    auto* acq_params = new AcqTaskParams();
    acq_params->imu = imu_adapter;
    acq_params->scheduler = telemetry_scheduler;
    acq_params->rc_switch = rc_adapter;

    bool boot_success = imu_healthy && lora_ready && sd_ready;

    if (boot_success) {
        // SUCCESS: 2 fast beeps
        infra::BuzzerCommand success_cmd = {2, 100, 100};
        xQueueSend(buzzer_queue, &success_cmd, 0);
    } else {
        // ERROR: 3 long beeps
        infra::BuzzerCommand error_cmd = {3, 500, 500};
        xQueueSend(buzzer_queue, &error_cmd, 0);
    }

    ESP_LOGI(TAG, "Starting RTOS Tasks...");

    xTaskCreatePinnedToCore(vTaskAcquisition, "AcqTask", 8192, (void*)acq_params, 5, NULL, 1);

    if (sd_ready) {
        xTaskCreatePinnedToCore(vTaskSDLogger, "SDTask", 8192, NULL, 3, NULL, 0);
    }

    // Start PWM logger to monitor RC input pulses
    xTaskCreatePinnedToCore(vTaskPWMLogger, "PWMLog", 2048, (void*)rc_adapter, 2, NULL, 0);

    xTaskCreatePinnedToCore(vTaskLoRaTX, "LoRaTask", 4096, (void*)lora_adapter, 4, NULL, 0);
}
