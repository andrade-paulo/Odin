#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

#include "orchestrator_task.hpp"
#include "ports/port_gps.hpp"

class GpsTask : public IGpsSensor {
public:
    GpsTask(OrchestratorTask* orchestrator, uart_port_t uartNum, int txPin, int rxPin);
    ~GpsTask(); // Este destrutor precisará invocar uart_driver_delete

    GpsTask(const GpsTask&) = delete;
    GpsTask& operator=(const GpsTask&) = delete;

    bool start();

    // Contratos da Port
    bool isHealthy() override;

private:
    OrchestratorTask* _orchestrator;
    uart_port_t _uartNum;
    int _txPin;
    int _rxPin;
    TaskHandle_t _taskHandle;

    static void taskEntry(void* pvParameters);
    void runLoop();

    // Configura o filtro interno do GPS para dinâmica de voo
    bool configureAirborneModel();
    
    // Função auxiliar para validar o Checksum de sentenças NMEA/UBX
    bool validateChecksum(const uint8_t* buffer, size_t len);
};