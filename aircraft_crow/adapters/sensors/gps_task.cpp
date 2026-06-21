#include "gps_task.hpp"
#include "esp_log.h"
#include <string.h>
#include <stdlib.h>

static const char* TAG = "GPS_TASK";

// O tamanho ideal para não perder pacotes se o FreeRTOS atrasar o escalonamento
#define UART_BUF_SIZE 1024

GpsTask::GpsTask(OrchestratorTask* orchestrator, uart_port_t uartNum, int txPin, int rxPin)
    : _orchestrator(orchestrator), 
      _uartNum(uartNum), 
      _txPin(txPin), 
      _rxPin(rxPin), 
      _taskHandle(nullptr) 
{
}

GpsTask::~GpsTask() {
    uart_driver_delete(_uartNum);
}

bool GpsTask::start() {
    // Configuração do Driver Nativo da UART
    uart_config_t uart_config = {};
    uart_config.baud_rate = 9600; // Baud rate padrão de fábrica do NEO-M8N
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.parity    = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config.source_clk = UART_SCLK_DEFAULT;

    if (uart_param_config(_uartNum, &uart_config) != ESP_OK) return false;
    if (uart_set_pin(_uartNum, _txPin, _rxPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE) != ESP_OK) return false;
    
    // Instala o driver com buffer de RX, mas sem buffer de TX (não vamos transmitir dados em massa)
    if (uart_driver_install(_uartNum, UART_BUF_SIZE, 0, 0, NULL, 0) != ESP_OK) return false;

    BaseType_t result = xTaskCreatePinnedToCore(
        &GpsTask::taskEntry,
        "Gps_Task",
        4096,
        this,
        3, // Prioridade 3 (Inferior à IMU e ao Barômetro, pois o GPS atualiza a 5-10Hz apenas)
        &_taskHandle,
        0  // Core 0
    );
    return (result == pdPASS);
}

void GpsTask::taskEntry(void* pvParameters) {
    GpsTask* instance = static_cast<GpsTask*>(pvParameters);
    instance->runLoop();
}

bool GpsTask::isHealthy() {
    // Na ausência de um ACK complexo, a saúde do GPS é verificada se a UART está operacional
    return uart_is_driver_installed(_uartNum);
}

bool GpsTask::configureAirborneModel() {
    ESP_LOGI(TAG, "Configurando U-blox para Airborne < 4G...");
    
    // Pacote UBX-CFG-NAV5 pré-calculado com dynModel = 8 (Airborne < 4g)
    // O NEO-M8N ignorará isso se o checksum (dois últimos bytes) estiver incorreto.
    const uint8_t ubx_cfg_nav5[] = {
        0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 
        0xFF, 0xFF, 0x08, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 
        0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x16, 0xDC 
    };

    int bytes_written = uart_write_bytes(_uartNum, (const char*)ubx_cfg_nav5, sizeof(ubx_cfg_nav5));
    vTaskDelay(pdMS_TO_TICKS(100)); // Aguarda o GPS processar a mudança
    
    return bytes_written == sizeof(ubx_cfg_nav5);
}

bool GpsTask::validateChecksum(const char* sentence) {
    if (sentence[0] != '$') return false;
    
    // O checksum NMEA é o XOR de todos os caracteres entre '$' e '*'
    uint8_t calculated_checksum = 0;
    int i = 1;
    while (sentence[i] != '*' && sentence[i] != '\0') {
        calculated_checksum ^= sentence[i];
        i++;
    }

    if (sentence[i] == '*') {
        int provided_checksum = (int)strtol(&sentence[i + 1], NULL, 16);
        return (calculated_checksum == provided_checksum);
    }
    return false;
}

void GpsTask::runLoop() {
    ESP_LOGI(TAG, "Inicializando GPS NEO-M8N...");

    if (!configureAirborneModel()) {
        ESP_LOGE(TAG, "Falha ao enviar pacote UBX. Continuando com modelo dinâmico padrão.");
    }

    uint8_t data[UART_BUF_SIZE];
    char lineBuffer[128]; // Buffer estático para montar uma sentença NMEA completa
    int lineIndex = 0;

    TelemetryDTO dto = {};
    dto.type = MessageType::GPS;

    while (true) {
        // A task bloqueia aqui. Ela só acorda (gastando CPU) quando houver bytes físicos na porta serial.
        // Timeout de 100ms para evitar travamentos infinitos.
        int len = uart_read_bytes(_uartNum, data, UART_BUF_SIZE - 1, pdMS_TO_TICKS(100));
        
        if (len > 0) {
            for (int i = 0; i < len; i++) {
                char c = (char)data[i];
                
                // Monta a sentença byte a byte
                if (c == '\n') {
                    lineBuffer[lineIndex] = '\0'; // Fecha a string C
                    
                    // Remove o \r se existir (Windows style)
                    if (lineIndex > 0 && lineBuffer[lineIndex - 1] == '\r') {
                        lineBuffer[lineIndex - 1] = '\0';
                    }

                    if (validateChecksum(lineBuffer)) {
                        // Como o módulo é M8N, ele usa o prefixo GNGGA (Múltiplas Constelações)
                        // ou GPGGA (Apenas GPS americano). Suportamos ambos.
                        if (strncmp(lineBuffer, "$GNGGA", 6) == 0 || strncmp(lineBuffer, "$GPGGA", 6) == 0) {
                            
                            char* saveptr;
                            // Ignora o header ($GNGGA)
                            strtok_r(lineBuffer, ",", &saveptr);
                            
                            // 1. Hora (ignoramos para performance, o ESP32 cuidará do timestamp)
                            strtok_r(NULL, ",", &saveptr);
                            
                            // 2. Latitude Bruta e 3. N/S
                            char* latStr = strtok_r(NULL, ",", &saveptr);
                            
                            // 4. Longitude Bruta e 5. E/W
                            char* lonStr = strtok_r(NULL, ",", &saveptr);
                            
                            // 6. Qualidade do Fix (0 = Sem sinal, 1 = Fix GPS, 2 = DGPS)
                            char* fixQuality = strtok_r(NULL, ",", &saveptr);
                            
                            // 7. Número de Satélites
                            char* sats = strtok_r(NULL, ",", &saveptr);
                            
                            // 8. HDOP (Ignorado por enquanto)
                            strtok_r(NULL, ",", &saveptr);
                            
                            // 9. Altitude e 10. Unidade (M)
                            char* altStr = strtok_r(NULL, ",", &saveptr);

                            if (fixQuality && atoi(fixQuality) > 0) {
                                dto.payload.gps.timestamp_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;
                                dto.payload.gps.satellites = atoi(sats);
                                dto.payload.gps.latitude = atof(latStr);   // Nota: O formato NMEA puro é DDMM.MMMMM
                                dto.payload.gps.longitude = atof(lonStr);  // Você terá que converter para Graus Decimais no Core
                                dto.payload.gps.altitude_msl = atof(altStr);

                                // Atira para o Orquestrador
                                _orchestrator->pushEvent(dto);
                            }
                        }
                        
                        // Parse da Velocidade de Solo (Ground Speed) vem na sentença RMC ou VTG
                        else if (strncmp(lineBuffer, "$GNRMC", 6) == 0 || strncmp(lineBuffer, "$GPRMC", 6) == 0) {
                            char* saveptr;
                            for (int skip = 0; skip < 7; skip++) {
                                strtok_r(skip == 0 ? lineBuffer : NULL, ",", &saveptr);
                            }
                            // O 7º parâmetro após o header é a velocidade em Nós (Knots)
                            char* speedStr = strtok_r(NULL, ",", &saveptr);
                            if (speedStr) {
                                // 1 Nó = 0.514444 m/s
                                dto.payload.gps.ground_speed_ms = atof(speedStr) * 0.514444f;
                                // Envia atualização com a velocidade
                                _orchestrator->pushEvent(dto);
                            }
                        }
                    }
                    
                    // Reseta o índice para a próxima linha
                    lineIndex = 0;
                } else if (lineIndex < sizeof(lineBuffer) - 1) {
                    lineBuffer[lineIndex++] = c;
                } else {
                    // Overflow de linha (lixo na serial), reseta a montagem
                    lineIndex = 0;
                }
            }
        }
    }
}