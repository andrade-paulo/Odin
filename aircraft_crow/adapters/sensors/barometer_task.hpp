#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "orchestrator_task.hpp"
#include "i2c_bus_manager.hpp"
#include "ports/port_sensor.hpp"

class BarometerTask : public ISensor {
public:
    BarometerTask(OrchestratorTask* orchestrator, I2cBusManager* i2cBus);
    ~BarometerTask() = default;

    BarometerTask(const BarometerTask&) = delete;
    BarometerTask& operator=(const BarometerTask&) = delete;

    bool start();

    // Contratos da Port
    bool isHealthy() override;
    void calibrate() override;

private:
    OrchestratorTask* _orchestrator;
    I2cBusManager* _i2cBus;
    TaskHandle_t _taskHandle;

    // Coeficientes de calibração internos do MS5611 (C1 a C6)
    uint16_t _calibCoeffs[6];

    // Estado do Filtro Exponential Moving Average (EMA)
    float _emaPressure;
    bool _emaInitialized;

    static void taskEntry(void* pvParameters);
    void runLoop();
    
    // Método privado para ler a PROM durante o setup da task
    bool readFactoryCalibration();
};