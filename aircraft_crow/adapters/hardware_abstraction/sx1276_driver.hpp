#pragma once

#include <cstdint>
#include <cstddef>
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class Sx1276Driver {
public:
    Sx1276Driver(spi_host_device_t spi_host, gpio_num_t cs, gpio_num_t rst, gpio_num_t dio0);
    ~Sx1276Driver();

    bool begin();

    // Retorna true se a transmissão foi confirmada pelo DIO0
    bool transmit(const uint8_t* buffer, size_t length);

private:
    spi_host_device_t _spi_host;
    spi_device_handle_t _spi;
    gpio_num_t _cs_pin;
    gpio_num_t _rst_pin;
    gpio_num_t _dio0_pin;

    SemaphoreHandle_t _txDoneSem;

    // Métodos de comunicação SPI de baixo nível
    void writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);
    void writeBuffer(uint8_t reg, const uint8_t* buffer, size_t length);

    // Rotina de interrupção em IRAM para resposta imediata
    static void IRAM_ATTR dio0InterruptHandler(void* arg);
};