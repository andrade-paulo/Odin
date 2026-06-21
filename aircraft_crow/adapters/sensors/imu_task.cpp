#include "imu_task.hpp"
#include "esp_log.h"

static const char* TAG = "IMU_TASK";

// O pino AD0 no esquemático está desconectado. Se o módulo físico 
// tiver pull-up interno, será 0x69. Se pull-down, 0x68.
static const uint8_t ICM_ADDRESS = 0x68; 
static const uint8_t REG_BANK_SEL = 0x7F;
static const uint8_t REG_PWR_MGMT_1 = 0x06;
static const uint8_t REG_ACCEL_XOUT_H = 0x2D;
static const uint8_t REG_WHO_AM_I = 0x00;
static const uint8_t EXPECTED_WHO_AM_I = 0xEA;

ImuTask::ImuTask(OrchestratorTask* orchestrator, I2cBusManager* i2cBus)
    : _orchestrator(orchestrator), 
      _i2cBus(i2cBus), 
      _taskHandle(nullptr), 
      _isCalibrating(false),
      _accelBiasX(0), _accelBiasY(0), _accelBiasZ(0),
      _gyroBiasX(0), _gyroBiasY(0), _gyroBiasZ(0) 
{
}

bool ImuTask::start() {
    BaseType_t result = xTaskCreatePinnedToCore(
        &ImuTask::taskEntry,
        "Imu_Task",
        4096,           
        this,           
        5,              // Prioridade Alta
        &_taskHandle,
        0               // Fixado no PRO_CPU (Core 0)
    );

    // Habilitar I2C Bypass no ICM-20948 para acessar o Magnetômetro
    uint8_t bypassCfg = 0x02; 
    _i2cBus->writeRegister(ICM_ADDRESS, 0x0F, bypassCfg); // REG_INT_PIN_CFG

    // Configurar AK09916 (Endereço 0x0C) para Modo Contínuo 100Hz
    uint8_t magMode = 0x08; 
    _i2cBus->writeRegister(0x0C, 0x31, magMode); // CNTL2 register

    return (result == pdPASS);
}

void ImuTask::taskEntry(void* pvParameters) {
    ImuTask* instance = static_cast<ImuTask*>(pvParameters);
    instance->runLoop();
}

void ImuTask::calibrate() {
    ESP_LOGI(TAG, "Comando de calibracao recebido. Sinalizando Core 0...");
    _isCalibrating.store(true);
}

bool ImuTask::isHealthy() {
    uint8_t whoAmI = 0;
    _i2cBus->writeRegister(ICM_ADDRESS, REG_BANK_SEL, 0x00);
    if (_i2cBus->readRegister(ICM_ADDRESS, REG_WHO_AM_I, whoAmI)) {
        return (whoAmI == EXPECTED_WHO_AM_I);
    }
    return false;
}

void ImuTask::runLoop() {
    ESP_LOGI(TAG, "Inicializando ICM-20948...");

    // Acorda o sensor (Limpa o sleep bit no banco 0)
    _i2cBus->writeRegister(ICM_ADDRESS, REG_BANK_SEL, 0x00);
    _i2cBus->writeRegister(ICM_ADDRESS, REG_PWR_MGMT_1, 0x01); // Auto clock selection
    vTaskDelay(pdMS_TO_TICKS(50));

    // Muda para o Banco 2 para configurar os Filtros em Hardware (DLPF)
    _i2cBus->writeRegister(ICM_ADDRESS, REG_BANK_SEL, 0x20); 
    
    // Configura Giroscópio: ±2000 dps, DLPF ativado
    _i2cBus->writeRegister(ICM_ADDRESS, 0x01, 0x07); 
    
    // Configura Acelerômetro: ±4g, DLPF ativado
    _i2cBus->writeRegister(ICM_ADDRESS, 0x14, 0x0F); 

    // Volta para o Banco 0 para a máquina de leitura contínua
    _i2cBus->writeRegister(ICM_ADDRESS, REG_BANK_SEL, 0x00);

    if (!isHealthy()) {
        ESP_LOGE(TAG, "Falha critica: ICM-20948 nao encontrado. Verifique se o endereço é 0x68 ou 0x69.");
        vTaskDelete(NULL); 
    }

    ESP_LOGI(TAG, "ICM-20948 pronto. Iniciando telemetria a 100Hz.");

    TickType_t lastWakeTime = xTaskGetTickCount();
    // Frequência de 100Hz = 10ms
    const TickType_t frequency = pdMS_TO_TICKS(10); 

    uint8_t buffer[12];

    while (true) {
        // Trava o loop para rodar com precisão cirúrgica a cada 10ms
        vTaskDelayUntil(&lastWakeTime, frequency);

        if (_isCalibrating.load()) {
            ESP_LOGI(TAG, "Amostrando bias (100 ciclos)...");
            int32_t sumAx = 0, sumAy = 0, sumAz = 0;
            int32_t sumGx = 0, sumGy = 0, sumGz = 0;

            for (int i = 0; i < 100; i++) {
                if (_i2cBus->readRegisters(ICM_ADDRESS, REG_ACCEL_XOUT_H, buffer, 12)) {
                    sumAx += (int16_t)((buffer[0] << 8) | buffer[1]);
                    sumAy += (int16_t)((buffer[2] << 8) | buffer[3]);
                    sumAz += (int16_t)((buffer[4] << 8) | buffer[5]);
                    sumGx += (int16_t)((buffer[6] << 8) | buffer[7]);
                    sumGy += (int16_t)((buffer[8] << 8) | buffer[9]);
                    sumGz += (int16_t)((buffer[10] << 8) | buffer[11]);
                }
                vTaskDelay(pdMS_TO_TICKS(5));
            }
            
            _accelBiasX = (float)sumAx / 100.0f;
            _accelBiasY = (float)sumAy / 100.0f;
            _accelBiasZ = ((float)sumAz / 100.0f) - 8192.0f; // Subtrai 1G (assumindo calibração com z para cima)
            _gyroBiasX = (float)sumGx / 100.0f;
            _gyroBiasY = (float)sumGy / 100.0f;
            _gyroBiasZ = (float)sumGz / 100.0f;

            _isCalibrating.store(false);
            ESP_LOGI(TAG, "Calibracao concluida. Retomando telemetria.");
            continue; 
        }

        // Leitura em Burst dos 12 bytes
        if (_i2cBus->readRegisters(ICM_ADDRESS, REG_ACCEL_XOUT_H, buffer, 12)) {
            
            int16_t rawAx = (buffer[0] << 8) | buffer[1];
            int16_t rawAy = (buffer[2] << 8) | buffer[3];
            int16_t rawAz = (buffer[4] << 8) | buffer[5];
            
            int16_t rawGx = (buffer[6] << 8) | buffer[7];
            int16_t rawGy = (buffer[8] << 8) | buffer[9];
            int16_t rawGz = (buffer[10] << 8) | buffer[11];

            TelemetryDTO dto = {};
            dto.type = MessageType::IMU;
            dto.payload.imu.timestamp_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;
            
            // Aceleração: (m/s²) * 100 -> int16_t
            dto.payload.imu.linear_acceleration_x = (int16_t)((((rawAx - _accelBiasX) / 8192.0f) * 9.81f) * 100.0f);
            dto.payload.imu.linear_acceleration_y = (int16_t)((((rawAy - _accelBiasY) / 8192.0f) * 9.81f) * 100.0f);
            dto.payload.imu.linear_acceleration_z = (int16_t)((((rawAz - _accelBiasZ) / 8192.0f) * 9.81f) * 100.0f);

            // Rotação: (°/s) * 10 -> int16_t
            dto.payload.imu.rotation_speed_x = (int16_t)((((rawGx - _gyroBiasX) / 131.0f)) * 10.0f);
            dto.payload.imu.rotation_speed_y = (int16_t)((((rawGy - _gyroBiasY) / 131.0f)) * 10.0f);
            dto.payload.imu.rotation_speed_z = (int16_t)((((rawGz - _gyroBiasZ) / 131.0f)) * 10.0f);

            // Leitura do Magnetômetro
            // Precisamos ler 7 bytes a partir de 0x11: HXL, HXH, HYL, HYH, HZL, HZH e o ST2 
            uint8_t magBuffer[7];
            if (_i2cBus->readRegisters(0x0C, 0x11, magBuffer, 7)) {
                // O AK09916 é Little Endian, diferente do ICM
                int16_t rawMagX = (magBuffer[1] << 8) | magBuffer[0];
                int16_t rawMagY = (magBuffer[3] << 8) | magBuffer[2];
                int16_t rawMagZ = (magBuffer[5] << 8) | magBuffer[4];
                
                // Obs.: O AK09916 entrega dados diretos em 0.15 µT/LSB
                // Estamos enviando os bits crus para preservar a escala
                // A conversão para µT ou mG pode ser feita na estação
                dto.payload.imu.magnetic_field_x = rawMagX;
                dto.payload.imu.magnetic_field_y = rawMagY;
                dto.payload.imu.magnetic_field_z = rawMagZ;
            }

            _orchestrator->pushEvent(dto);
        } else {
            ESP_LOGW(TAG, "Falha na leitura em burst do ICM-20948.");
        }
    }
}