#include "barometer_task.hpp"
#include "esp_log.h"

static const char* TAG = "BARO_TASK";

static const uint8_t MS5611_ADDR = 0x77;
static const uint8_t CMD_RESET = 0x1E;
static const uint8_t CMD_PROM_READ_BASE = 0xA0;
static const uint8_t CMD_CONVERT_D1_4096 = 0x48; // Pressão (Máxima Resolução)
static const uint8_t CMD_CONVERT_D2_4096 = 0x58; // Temperatura (Máxima Resolução)
static const uint8_t CMD_ADC_READ = 0x00;

BarometerTask::BarometerTask(OrchestratorTask* orchestrator, I2cBusManager* i2cBus)
    : _orchestrator(orchestrator), 
      _i2cBus(i2cBus), 
      _taskHandle(nullptr),
      _emaPressure(0.0f),
      _emaInitialized(false)
{
    for(int i=0; i<6; i++) _calibCoeffs[i] = 0;
}

bool BarometerTask::start() {
    BaseType_t result = xTaskCreatePinnedToCore(
        &BarometerTask::taskEntry,
        "Baro_Task",
        4096,
        this,
        4, // Prioridade ligeiramente menor que a IMU
        &_taskHandle,
        0  // Core 0
    );
    return (result == pdPASS);
}

void BarometerTask::taskEntry(void* pvParameters) {
    BarometerTask* instance = static_cast<BarometerTask*>(pvParameters);
    instance->runLoop();
}

bool BarometerTask::isHealthy() {
    // Um simples reset e delay serve como health check se o I2C responder
    return _i2cBus->writeCommand(MS5611_ADDR, CMD_RESET);
}

void BarometerTask::calibrate() { return; }

bool BarometerTask::readFactoryCalibration() {
    uint8_t buffer[2];
    // O MS5611 tem 6 coeficientes principais (C1 a C6) localizados em 0xA2 até 0xAC
    for (uint8_t i = 0; i < 6; i++) {
        uint8_t promAddr = CMD_PROM_READ_BASE + ((i + 1) * 2);
        if (!_i2cBus->readRegisters(MS5611_ADDR, promAddr, buffer, 2)) {
            return false;
        }
        _calibCoeffs[i] = (buffer[0] << 8) | buffer[1];
    }
    return true;
}

void BarometerTask::runLoop() {
    ESP_LOGI(TAG, "Inicializando MS5611...");

    if (!_i2cBus->writeCommand(MS5611_ADDR, CMD_RESET)) {
        ESP_LOGE(TAG, "Falha ao resetar MS5611.");
        vTaskDelete(NULL);
    }
    vTaskDelay(pdMS_TO_TICKS(10)); // Aguarda o chip reiniciar

    if (!readFactoryCalibration()) {
        ESP_LOGE(TAG, "Falha ao ler coeficientes da PROM.");
        vTaskDelete(NULL);
    }
    
    ESP_LOGI(TAG, "MS5611 Pronto. C1:%d C2:%d C3:%d C4:%d C5:%d C6:%d", 
             _calibCoeffs[0], _calibCoeffs[1], _calibCoeffs[2], 
             _calibCoeffs[3], _calibCoeffs[4], _calibCoeffs[5]);

    uint8_t buffer[3]; // O ADC do MS5611 devolve 24 bits (3 bytes)
    const float EMA_ALPHA = 0.2f; // Fator de suavização (0.0 a 1.0). Altere conforme o ruído do vento.

    while (true) {
        // Leitura da pressão
        _i2cBus->writeCommand(MS5611_ADDR, CMD_CONVERT_D1_4096);
        vTaskDelay(pdMS_TO_TICKS(10)); // Libera a CPU por exatos 10ms (ADC requer ~9.04ms)
        
        uint32_t D1 = 0;
        if (_i2cBus->readRegisters(MS5611_ADDR, CMD_ADC_READ, buffer, 3)) {
            D1 = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
        }

        // Leitura da temperatura
        _i2cBus->writeCommand(MS5611_ADDR, CMD_CONVERT_D2_4096);
        vTaskDelay(pdMS_TO_TICKS(10)); // Libera a CPU novamente
        
        uint32_t D2 = 0;
        if (_i2cBus->readRegisters(MS5611_ADDR, CMD_ADC_READ, buffer, 3)) {
            D2 = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
        }

        if (D1 > 0 && D2 > 0) {
            int32_t dT = D2 - ((uint32_t)_calibCoeffs[4] << 8);
            int32_t TEMP = 2000 + (((int64_t)dT * _calibCoeffs[5]) >> 23);

            int64_t OFF = ((int64_t)_calibCoeffs[1] << 16) + (((int64_t)_calibCoeffs[3] * dT) >> 7);
            int64_t SENS = ((int64_t)_calibCoeffs[0] << 15) + (((int64_t)_calibCoeffs[2] * dT) >> 8);

            int32_t P = (((D1 * SENS) >> 21) - OFF) >> 15;

            float currentPressure_Pa = (float)P; // Declarado como float para o cálculo do EMA

            if (!_emaInitialized) {
                _basePressure = currentPressure_Pa; // Salva pressão inicial no solo
                _emaPressure = currentPressure_Pa;
                _emaInitialized = true;
            } else {
                _emaPressure = (EMA_ALPHA * currentPressure_Pa) + ((1.0f - EMA_ALPHA) * _emaPressure);
            }
            
            TelemetryDTO dto = {};
            dto.type = MessageType::BARO;
            dto.payload.barometer.timestamp_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;
            
            // Quantização Delta Pressão (Pa) -> int16_t
            dto.payload.barometer.pressure_delta = (int16_t)(_emaPressure - _basePressure);
            // Quantização da Temperatura: °C * 100 -> int16_t
            dto.payload.barometer.temperature = (int16_t)TEMP;

            _orchestrator->pushEvent(dto);
        } else {
            ESP_LOGW(TAG, "Falha ao ler ADC do MS5611. Ignorando ciclo.");
        }
    }
}