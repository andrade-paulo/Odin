#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_log.h"

#include "telemetry_dto.hpp"
#include "orchestrator_task.hpp" 


// Forward declaration para evitar dependência circular com o componente main
class OrchestratorTask;

class ProducerUartHilTask {
public:
    // Utiliza a UART2 por padrão (Pinos do GPS no esquemático: RX=16, TX=17)
    explicit ProducerUartHilTask(OrchestratorTask* orchestratorTask, uart_port_t uartNum = UART_NUM_2);
    ~ProducerUartHilTask();

    // Inicializa a UART e levanta a task no Core 0
    bool start();

private:
    OrchestratorTask* _orchestratorTask;
    uart_port_t _uartNum;
    TaskHandle_t _taskHandle;

    static void taskEntry(void* pvParameters);
    void runLoop();
    void parseAndPush(char* line);
};