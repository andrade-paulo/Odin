#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

// Includes da Camada de Domínio e Comunicação
#include "telemetry_dto.hpp"
#include "lora_task.hpp"

static const char* TAG = "TX_TEST";

// Defina a pinagem do seu Módulo LoRa no ESP32 Transmissor
#define LORA_SCK_PIN  18
#define LORA_MISO_PIN 19
#define LORA_MOSI_PIN 23
#define LORA_CS_PIN   5
#define LORA_RST_PIN  14
#define LORA_DIO0_PIN 26

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "=== Iniciando Teste de Transmissao LoRa (RF Ping) ===");

    // 1. Inicializar o Barramento SPI Global (VSPI)
    spi_bus_config_t buscfg = {};
    buscfg.miso_io_num = LORA_MISO_PIN;
    buscfg.mosi_io_num = LORA_MOSI_PIN;
    buscfg.sclk_io_num = LORA_SCK_PIN;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    buscfg.max_transfer_sz = 4096;

    // Usando SPI_DMA_CH_AUTO para compatibilidade com ESP-IDF moderno
    esp_err_t ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Falha critica ao inicializar o barramento SPI. Erro: %d", ret);
        return;
    }

    // 2. Instanciar e Iniciar a Task do LoRa
    // Parâmetros: SPI Host, CS, RST, DIO0, System ID, Component ID
    LoRaTask loraTask(SPI2_HOST, LORA_CS_PIN, LORA_RST_PIN, LORA_DIO0_PIN, 1, 1);
    
    if (!loraTask.start()) {
        ESP_LOGE(TAG, "Falha ao alocar a task LoRa no FreeRTOS.");
        return;
    }

    ESP_LOGI(TAG, "LoRaTask iniciada com sucesso. Iniciando injecao de pacotes...");

    // 3. Injeção de Dados Simulados (Mock do Orquestrador)
    while (true) {
        // --- Pacote Completo (Sem compressão) ---
        TelemetryDTO dto_full = {};
        dto_full.type = MessageType::IMU;
        dto_full.payload.imu.timestamp_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;
        dto_full.payload.imu.linear_acceleration_x = 981;
        dto_full.payload.imu.linear_acceleration_y = 120;
        dto_full.payload.imu.linear_acceleration_z = -981;
        dto_full.payload.imu.rotation_speed_x = 1500;
        dto_full.payload.imu.rotation_speed_y = -100;
        dto_full.payload.imu.rotation_speed_z = 50;
        dto_full.payload.imu.magnetic_field_x = 300;
        dto_full.payload.imu.magnetic_field_y = -300;
        dto_full.payload.imu.magnetic_field_z = 100;

        ESP_LOGI(TAG, "Despachando DTO Completo (Espera-se ~33 bytes no ar)...");
        loraTask.sendTelemetry(dto_full);

        // Aguarda 2 segundos para não colapsar o rádio e facilitar a leitura no receptor
        vTaskDelay(pdMS_TO_TICKS(2000)); 

        // --- Pacote Truncado (Muitos zeros) ---
        TelemetryDTO dto_zero = {};
        dto_zero.type = MessageType::IMU;
        dto_zero.payload.imu.timestamp_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;
        // Restante zerado para forçar o Zero-Byte Truncation do MAVLink v2
        dto_zero.payload.imu.linear_acceleration_x = 0; 
        dto_zero.payload.imu.linear_acceleration_y = 0;
        dto_zero.payload.imu.linear_acceleration_z = 0;
        dto_zero.payload.imu.rotation_speed_x = 0;     
        dto_zero.payload.imu.rotation_speed_y = 0;
        dto_zero.payload.imu.rotation_speed_z = 0;
        dto_zero.payload.imu.magnetic_field_x = 0;
        dto_zero.payload.imu.magnetic_field_y = 0;
        dto_zero.payload.imu.magnetic_field_z = 0;

        ESP_LOGI(TAG, "Despachando DTO Truncado (Espera-se ~14 bytes no ar)...");
        loraTask.sendTelemetry(dto_zero);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}