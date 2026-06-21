#pragma once

#include <cstdint>
#include <cstddef>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/i2c.h"

class I2cBusManager {
public:
    // Construtor recebe a configuração física do barramento
    I2cBusManager(i2c_port_t port, int sdaPin, int sclPin, uint32_t clockSpeedHz);
    ~I2cBusManager();

    // Bloqueia cópia e atribuição para garantir que o hardware tenha um único dono lógico
    I2cBusManager(const I2cBusManager&) = delete;
    I2cBusManager& operator=(const I2cBusManager&) = delete;

    // Inicializa os pinos, instala o driver nativo e cria o Mutex
    bool init();

    // API Transacional Thread-Safe
    bool writeRegister(uint8_t deviceAddress, uint8_t regAddress, uint8_t data);
    bool readRegister(uint8_t deviceAddress, uint8_t regAddress, uint8_t& data);
    bool readRegisters(uint8_t deviceAddress, uint8_t regAddress, uint8_t* dataBuffer, size_t length);
    bool writeCommand(uint8_t deviceAddress, uint8_t command);

private:
    i2c_port_t _port;
    int _sdaPin;
    int _sclPin;
    uint32_t _clockSpeedHz;

    // O "cadeado" do barramento. Impede que IMU e Barômetro colidam.
    SemaphoreHandle_t _mutex;

    // Tempo máximo que uma task pode esperar para pegar o barramento (ex: 10ms)
    // Se o barramento travar por hardware, isso evita que a task congele para sempre
    const TickType_t _timeoutTicks; 
};