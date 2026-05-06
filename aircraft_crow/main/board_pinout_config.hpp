#pragma once
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"

namespace BoardConfig {

    // --- I2C Bus for MPU6050 ---
    namespace I2C_SensorBus {
        constexpr i2c_port_t PORT = I2C_NUM_0;
        constexpr gpio_num_t SDA  = GPIO_NUM_21;
        constexpr gpio_num_t SCL  = GPIO_NUM_26;
        constexpr uint32_t   FREQ = 400000; // 400 kHz Fast Mode
    }

    // --- Shared Hardware SPI Pins ---
    namespace SPI_Shared {
        constexpr spi_host_device_t HOST = SPI3_HOST; // Use VSPI for both
        constexpr gpio_num_t MISO = GPIO_NUM_19;
        constexpr gpio_num_t MOSI = GPIO_NUM_23;
        constexpr gpio_num_t CLK  = GPIO_NUM_18;
    }

    // --- LoRa (SX1276) ---
    namespace SPI_LoRa {
        constexpr gpio_num_t CS   = GPIO_NUM_5;
        constexpr gpio_num_t RST  = GPIO_NUM_14;
        constexpr gpio_num_t DIO0 = GPIO_NUM_32; // TX Done Interrupt
    }

    // --- SD Card ---
    namespace SPI_SDCard {
        constexpr gpio_num_t CS   = GPIO_NUM_27;
    }

    // --- System Status ---
    namespace Status {
        constexpr gpio_num_t BUZZER = GPIO_NUM_4;
    }

    namespace RC_Input {
        constexpr gpio_num_t PWM_RX = GPIO_NUM_13;
    }
} // namespace BoardConfig
