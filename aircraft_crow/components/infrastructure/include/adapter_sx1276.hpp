#pragma once
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace infra {

class SX1276_Adapter {
private:
    spi_host_device_t spi_host;
    spi_device_handle_t spi_device;
    gpio_num_t pin_cs;
    gpio_num_t pin_reset;
    gpio_num_t pin_dio0; // Interrupt pin for TX_DONE

    SemaphoreHandle_t tx_done_sem;

    bool initialized_flag = false;

    // Internal SPI helpers
    void write_register(uint8_t reg, uint8_t data);
    void write_fifo(const uint8_t* data, size_t length);

    // Static ISR handler (ESP-IDF requires static/C-linkage for ISRs)
    static void IRAM_ATTR dio0_isr_handler(void* arg);

public:
    // Takes an already-initialized SPI bus ID (so it can share with the SD Card)
    SX1276_Adapter(spi_host_device_t host, int cs, int reset, int dio0);
    ~SX1276_Adapter();

    bool initialize();
    
    // Non-blocking from CPU perspective: Loads FIFO, triggers TX, waits on Semaphore
    bool transmit_packet_blocking(const uint8_t* data, size_t length);

    bool is_initialized() const { return initialized_flag; }
};

}
