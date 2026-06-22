#include "consumer_uart_hil_task.hpp"
#include "esp_log.h"
#include "driver/uart.h"
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

    uart_config_t uart_config = {}; // Zera toda a memória da struct (cobre qualquer versão do ESP-IDF)
    uart_config.baud_rate = 115200;
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.parity = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config.source_clk = UART_SCLK_DEFAULT;
    
    // Verifique se o driver já não está instalado pelo ESP_LOG
    if (!uart_is_driver_installed(_uartNum)) {
        ESP_ERROR_CHECK(uart_driver_install(_uartNum, 256, 0, 0, NULL, 0));
        ESP_ERROR_CHECK(uart_param_config(_uartNum, &uart_config));
        ESP_ERROR_CHECK(uart_set_pin(_uartNum, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    }

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
    // Chamado pelo Core (Core 1). O timeout 0 garante que o Domínio puro 
    // nunca fique bloqueado caso o cabo serial trave.
    if (_txQueue) {
        xQueueSend(_txQueue, &packet, 0); 
    }
}

void ConsumerUartHilTask::taskEntry(void* pvParameters) {
    ConsumerUartHilTask* instance = static_cast<ConsumerUartHilTask*>(pvParameters);
    instance->runLoop();
}

void ConsumerUartHilTask::runLoop() {
    TelemetryDTO packet;
    char txBuffer[256]; // Tamanho aumentado para comportar strings de GPS grandes

    ESP_LOGI(TAG, "Consumer HIL Task iniciada no Core 0.");

    while (true) {
        // Aguarda indefinidamente até que o Orchestrator empurre um pacote para a porta
        if (xQueueReceive(_txQueue, &packet, portMAX_DELAY) == pdTRUE) {
            int len = 0;

            // Serializa o DTO estruturado de volta para uma string CSV legível no PC
            if (packet.type == MessageType::IMU) {
                // IMU, timestamp, Ax, Ay, Az, Gx, Gy, Gz
                len = snprintf(txBuffer, sizeof(txBuffer), "IMU,%lu,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                               packet.payload.imu.timestamp_ms,
                               packet.payload.imu.linear_acceleration_x,
                               packet.payload.imu.linear_acceleration_y,
                               packet.payload.imu.linear_acceleration_z,
                               packet.payload.imu.rotation_speed_x,
                               packet.payload.imu.rotation_speed_y,
                               packet.payload.imu.rotation_speed_z,
                               packet.payload.imu.magnetic_field_x,
                               packet.payload.imu.magnetic_field_y,
                               packet.payload.imu.magnetic_field_z);
                               
            } else if (packet.type == MessageType::BARO) {
                // BARO, timestamp, Pressao (Pa), Temp (C)
                len = snprintf(txBuffer, sizeof(txBuffer), "BARO,%lu,%d,%d\n",
                               packet.payload.barometer.timestamp_ms,
                               packet.payload.barometer.pressure_delta,
                               packet.payload.barometer.temperature);
                               
            } else if (packet.type == MessageType::GPS) {
                // GPS, timestamp, sats, Lat, Lon, Alt (msl), Ground Speed (m/s)
                len = snprintf(txBuffer, sizeof(txBuffer), "GPS,%lu,%d,%lu,%lu,%d,%d\n",
                               packet.payload.gps.timestamp_ms,
                               packet.payload.gps.satellites,
                               packet.payload.gps.latitude,
                               packet.payload.gps.longitude,
                               packet.payload.gps.altitude_msl,
                               packet.payload.gps.ground_speed_ms);
            }

            // Escreve no buffer físico da UART
            if (len > 0) {
                uart_write_bytes(_uartNum, txBuffer, len);
            }
        }
    }
}