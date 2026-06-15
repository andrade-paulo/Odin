#include "consumer_uart_hil_task.hpp"
#include "esp_log.h"
#include <stdio.h>
#include <string.h>

static const char* TAG = "HIL_CONSUMER";

constexpr uint32_t TX_QUEUE_LENGTH = 30;
constexpr uint32_t CONSUMER_STACK_SIZE = 4096;
constexpr uint8_t CONSUMER_PRIORITY = 5;
constexpr BaseType_t CORE_0 = 0;

ConsumerUartHilTask::ConsumerUartHilTask(uart_port_t uartNum)
    : _uartNum(uartNum), _taskHandle(nullptr) {
    _txQueue = xQueueCreate(TX_QUEUE_LENGTH, sizeof(TelemetryDTO));
}

ConsumerUartHilTask::~ConsumerUartHilTask() {
    if (_txQueue) vQueueDelete(_txQueue);
    if (_taskHandle) vTaskDelete(_taskHandle);
}

bool ConsumerUartHilTask::start() {
    if (!_txQueue) return false;

    BaseType_t result = xTaskCreatePinnedToCore(
        ConsumerUartHilTask::taskEntry,
        "ConsumerUartHilTask",
        CONSUMER_STACK_SIZE,
        this,
        CONSUMER_PRIORITY,
        &_taskHandle,
        CORE_0
    );

    return result == pdPASS;
}

void ConsumerUartHilTask::sendPacket(const TelemetryDTO& packet) {
    if (_txQueue) {
        xQueueSend(_txQueue, &packet, 0); // Timeout 0 para não travar o Core
    }
}

void ConsumerUartHilTask::taskEntry(void* pvParameters) {
    ConsumerUartHilTask* instance = static_cast<ConsumerUartHilTask*>(pvParameters);
    instance->runLoop();
}

void ConsumerUartHilTask::runLoop() {
    TelemetryDTO packet;
    char txBuffer[128];

    ESP_LOGI(TAG, "Consumer HIL Task iniciada no Core 0.");

    while (true) {
        // Aguarda indefinidamente até que o Orchestrator empurre um pacote
        if (xQueueReceive(_txQueue, &packet, portMAX_DELAY) == pdTRUE) {
            int len = 0;

            // Serializa o DTO estruturado de volta para uma string legível no HIL
            if (packet.type == MessageType::IMU) {
                len = snprintf(txBuffer, sizeof(txBuffer), "TX_IMU,%lu,%.2f,%.2f,%.2f\n",
                               packet.payload.imu.timestamp_ms,
                               packet.payload.imu.linear_acceleration_x,
                               packet.payload.imu.linear_acceleration_y,
                               packet.payload.imu.linear_acceleration_z);
            } else if (packet.type == MessageType::BARO) {
                len = snprintf(txBuffer, sizeof(txBuffer), "TX_BARO,%lu,%.2f,%.2f\n",
                packet.payload.barometer.timestamp_ms,
                packet.payload.barometer.altitude,
                packet.payload.barometer.pressure);
            }

            // Escreve no buffer físico da UART
            if (len > 0) {
                uart_write_bytes(_uartNum, txBuffer, len);
            }
        }
    }
}