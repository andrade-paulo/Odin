#pragma once
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_attr.h"


namespace infra {

class RCReceiverAdapter {
private:
    gpio_num_t pin_pwm;
    
    // Volatile because they are modified inside the ISR
    volatile uint32_t pulse_start_time = 0;
    volatile uint32_t pulse_width_us = 0;

    static void IRAM_ATTR gpio_isr_handler(void* arg);

public:
    RCReceiverAdapter(gpio_num_t pin);
    bool initialize();

    // Returns true if the PWM pulse is > 1500 microseconds (Switch is ON)
    bool is_recording_active();

    // Return the last measured pulse width in microseconds
    uint32_t get_pulse_width_us() const { return pulse_width_us; }
};

}
