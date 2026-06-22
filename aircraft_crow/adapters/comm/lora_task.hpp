#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "ports/port_telemetry_sender.hpp"
#include "telemetry_dto.hpp"
#include "mavlink_wrapper.hpp"

class LoRaTask : public ITelemetrySender {
public:
    // O construtor recebe os pinos e os IDs do sistema MAVLink
    LoRaTask(int spi_host, int cs_pin, int rst_pin, int dio0_pin, uint8_t sys_id, uint8_t comp_id);
    
    bool start();
    
    // Método exposto para o Orquestrador injetar os dados
    void sendPacket(const TelemetryDTO& dto) override;

private:
    QueueHandle_t _txQueue;
    TaskHandle_t _taskHandle;
    MavlinkWrapper _mavlink;
    
    // Pinos de controle
    int _spi_host;
    int _cs_pin;
    int _rst_pin;
    int _dio0_pin;

    static void taskEntry(void* pvParameters);
    void run();
    
    // Inicialização do rádio SX1276
    bool initRadio();
};