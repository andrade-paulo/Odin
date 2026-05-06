#pragma once
#include "driver/gpio.h"
#include "driver/ledc.h"

namespace infra {

// The command payload sent through the FreeRTOS Queue
struct BuzzerCommand {
    int beeps;
    int duration_on_ms;
    int duration_off_ms;
};

class BuzzerAdapter {
private:
    gpio_num_t pin;

public:
    BuzzerAdapter(gpio_num_t buzzer_pin);
    bool initialize();
    
    // Turns the 2000Hz PWM tone ON or OFF
    void set_state(bool on);
};

}
