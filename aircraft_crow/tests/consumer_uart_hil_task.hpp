#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"

#include "port_telemetry_sender.hpp"
#include "telemetry_dto.hpp"

class ConsumerUartHilTask : public ITelemetrySender {
public:
    explicit ConsumerUartHilTask(uart_port_t uartNum = UART_NUM_0);
    ~ConsumerUartHilTask();

    bool start();

    // Implementação obrigatória da interface de saída
    void sendPacket(const TelemetryDTO& packet) override;

private:
    uart_port_t _uartNum;
    QueueHandle_t _txQueue;
    TaskHandle_t _taskHandle;

    static void taskEntry(void* pvParameters);
    void runLoop();
};