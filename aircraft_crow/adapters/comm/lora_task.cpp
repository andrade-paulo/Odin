#include "lora_task.hpp"
#include "sx1276_driver.hpp"
#include "esp_log.h"

static const char* TAG = "LORA_TASK";

#define LORA_QUEUE_SIZE 15


LoRaTask::LoRaTask(int spi_host, int cs_pin, int rst_pin, int dio0_pin, uint8_t sys_id, uint8_t comp_id)
    : _taskHandle(nullptr),
      _mavlink(sys_id, comp_id),
      _spi_host(spi_host), 
      _cs_pin(cs_pin), 
      _rst_pin(rst_pin), 
      _dio0_pin(dio0_pin)
{
    _txQueue = xQueueCreate(LORA_QUEUE_SIZE, sizeof(TelemetryDTO));
    if (_txQueue == nullptr) {
        ESP_LOGE(TAG, "Falha ao criar a fila do LoRa.");
    }
}

bool LoRaTask::start() {
    if (_txQueue == nullptr) return false;

    BaseType_t res = xTaskCreatePinnedToCore(
        &LoRaTask::taskEntry,
        "LoRa_TX_Task",
        4096,
        this,
        3, // Prioridade média
        &_taskHandle,
        1  // Fixado no Core 1
    );

    return (res == pdPASS);
}

void LoRaTask::sendPacket(const TelemetryDTO& dto) {
    if (_txQueue != nullptr) {
        // Obs.: Se o LoRa estiver ocupado e a fila encher, o pacote actual é descartado para não travar o Orquestrador.
        if (xQueueSend(_txQueue, &dto, 0) != pdPASS) {
            // ESP_LOGD(TAG, "Fila LoRa cheia. Pacote descartado.");
        }
    }
}

void LoRaTask::taskEntry(void* pvParameters) {
    LoRaTask* instance = static_cast<LoRaTask*>(pvParameters);
    instance->run();
}

void LoRaTask::run() {
    // Instancia o Driver de Hardware apenas no contexto da task
    Sx1276Driver radio(
        static_cast<spi_host_device_t>(_spi_host), 
        static_cast<gpio_num_t>(_cs_pin), 
        static_cast<gpio_num_t>(_rst_pin), 
        static_cast<gpio_num_t>(_dio0_pin)
    );

    // Tenta inicializar o barramento e o chip
    if (!radio.begin()) {
        ESP_LOGE(TAG, "Falha de hardware: Rádio LoRa não inicializou. Task abortada.");
        vTaskDelete(NULL); 
        return;
    }

    ESP_LOGI(TAG, "LoRa Task iniciada e rádio pronto para transmissão.");

    TelemetryDTO incomingDto;
    uint8_t tx_buffer[280]; // 280 bytes estáticos alocados na stack

    // Loop Infinito de Transmissão
    while (true) {
        // Bloqueia permanentemente até chegar um DTO do Orquestrador
        if (xQueueReceive(_txQueue, &incomingDto, portMAX_DELAY) == pdTRUE) {
            
            // Serialização MAVLink
            size_t payload_length = _mavlink.serialize(incomingDto, tx_buffer, sizeof(tx_buffer));

            // Despacho SPI
            if (payload_length > 0) {
                if (!radio.transmit(tx_buffer, payload_length)) {
                    ESP_LOGW(TAG, "Erro na transmissão LoRa (Timeout ou interrupção perdida).");
                }
            } else {
                ESP_LOGE(TAG, "Erro no MAVLink: Falha na serialização do pacote.");
            }
        }
    }
}