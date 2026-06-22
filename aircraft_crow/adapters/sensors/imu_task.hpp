#pragma once

#include <atomic>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Injeções de Dependência
#include "orchestrator_task.hpp"
#include "i2c_bus_manager.hpp"

// A Port que esta Task implementa (Driven Adapter)
#include "ports/port_sensor.hpp"

class ImuTask : public ISensor {
public:
    ImuTask(OrchestratorTask* orchestrator, I2cBusManager* i2cBus);
    ~ImuTask() = default;

    ImuTask(const ImuTask&) = delete;
    ImuTask& operator=(const ImuTask&) = delete;

    // Arranca a Task no FreeRTOS (Core 0)
    bool start();

    // Contratos da Port (Chamados pelo Core 1)
    void calibrate() override;
    bool isHealthy() override;

private:
    OrchestratorTask* _orchestrator;
    I2cBusManager* _i2cBus;
    TaskHandle_t _taskHandle;

    // Proteção Atómica para concorrência Core 0 <-> Core 1
    std::atomic<bool> _isCalibrating;
    
    // Variáveis de Bias (calculadas na calibração e subtraídas nas leituras)
    float _accelBiasX, _accelBiasY, _accelBiasZ;
    float _gyroBiasX, _gyroBiasY, _gyroBiasZ;

    static void taskEntry(void* pvParameters);
    void runLoop();
};