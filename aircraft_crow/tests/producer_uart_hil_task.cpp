#include "producer_uart_hil_task.hpp"


static const char* TAG = "HIL_EMULATOR";

constexpr uint32_t HIL_TASK_STACK_SIZE = 4096;
constexpr uint8_t HIL_TASK_PRIORITY = 6; // Prioridade alta para não perder pacotes seriais
constexpr BaseType_t CORE_0 = 0;         // Fixado no PRO_CPU para não travar o FSM

ProducerUartHilTask::ProducerUartHilTask(OrchestratorTask* orchestratorTask, uart_port_t uartNum)
    : _orchestratorTask(orchestratorTask), _uartNum(uartNum), _taskHandle(nullptr) {}

ProducerUartHilTask::~ProducerUartHilTask() {
    if (_taskHandle != nullptr) {
        vTaskDelete(_taskHandle);
    }
    uart_driver_delete(_uartNum);
}

bool ProducerUartHilTask::start() {
    // Zero-initialize the entire structure first to prevent missing initializer errors
    uart_config_t uart_config = {};
    
    // Assign the specific fields
    uart_config.baud_rate = 115200;
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.parity    = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config.source_clk = UART_SCLK_APB;

    ESP_ERROR_CHECK(uart_driver_install(_uartNum, 1024, 1024, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(_uartNum, &uart_config));
    
    ESP_ERROR_CHECK(uart_set_pin(_uartNum, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    BaseType_t result = xTaskCreatePinnedToCore(
        ProducerUartHilTask::taskEntry,
        "ProducerUartHilTask",
        HIL_TASK_STACK_SIZE,
        this,
        HIL_TASK_PRIORITY,
        &_taskHandle,
        CORE_0
    );

    return result == pdPASS;
}

void ProducerUartHilTask::taskEntry(void* pvParameters) {
    ProducerUartHilTask* instance = static_cast<ProducerUartHilTask*>(pvParameters);
    instance->runLoop();
}

void ProducerUartHilTask::runLoop() {
    uint8_t data[256];
    char lineBuffer[256];
    int linePos = 0;

    ESP_LOGI(TAG, "HIL Task iniciada no Core 0. Aguardando dados na UART0");

    while (true) {
        // Bloqueia com pequeno timeout aguardando pacotes do PC
        int length = uart_read_bytes(_uartNum, data, sizeof(data) - 1, pdMS_TO_TICKS(10));
        
        if (length > 0) {
            for (int i = 0; i < length; i++) {
                char c = (char)data[i];
                // Detecta quebra de linha (fim da instrução do simulador)
                if (c == '\n' || c == '\r') {
                    if (linePos > 0) {
                        lineBuffer[linePos] = '\0';
                        parseAndPush(lineBuffer);
                        linePos = 0;
                    }
                } else if (linePos < sizeof(lineBuffer) - 1) {
                    lineBuffer[linePos++] = c;
                }
            }
        }
    }
}

void ProducerUartHilTask::parseAndPush(char* line) {
    TelemetryDTO dto;
    
    // Obtém o primeiro token (o cabeçalho da mensagem)
    char* typeStr = strtok(line, ",");
    if (typeStr == nullptr) return;

    // Ex: "CMD,START"
    if (strcmp(typeStr, "CMD") == 0) {
        char* cmdStr = strtok(NULL, ",");
        if (cmdStr != nullptr) {
            dto.type = MessageType::COMMAND;
            dto.payload.command = (strcmp(cmdStr, "START") == 0) ? 
            CommandType::START_RECORDING : CommandType::STOP_RECORDING;
            _orchestratorTask->pushEvent(dto);
        }
    } 
    // Ex: "IMU,1234,9.81,0.0,0.0,0.0,0.0,0.0"
    else if (strcmp(typeStr, "IMU") == 0) {
        dto.type = MessageType::IMU;
        dto.payload.imu.timestamp_ms = (uint32_t)atoi(strtok(NULL, ","));
        dto.payload.imu.linear_acceleration_x = atof(strtok(NULL, ","));
        dto.payload.imu.linear_acceleration_y = atof(strtok(NULL, ","));
        dto.payload.imu.linear_acceleration_z = atof(strtok(NULL, ","));
        dto.payload.imu.rotation_speed_x = atof(strtok(NULL, ","));
        dto.payload.imu.rotation_speed_y = atof(strtok(NULL, ","));
        dto.payload.imu.rotation_speed_z = atof(strtok(NULL, ","));
        
        // Empurra o evento para a Fila do FreeRTOS
        //ESP_LOGI(TAG, "IMU event");
        _orchestratorTask->pushEvent(dto);
    }
    // Ex: "BARO,1234,150.5,101325.0"
    else if (strcmp(typeStr, "BARO") == 0) {
        dto.type = MessageType::BARO;
        dto.payload.barometer.timestamp_ms = (uint32_t)atoi(strtok(NULL, ","));
        dto.payload.barometer.altitude = atof(strtok(NULL, ","));
        dto.payload.barometer.pressure = atof(strtok(NULL, ","));
        
        //ESP_LOGI(TAG, "BARO event");
        _orchestratorTask->pushEvent(dto);
    }
}