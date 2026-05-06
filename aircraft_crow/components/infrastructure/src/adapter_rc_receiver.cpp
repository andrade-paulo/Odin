#include "adapter_rc_receiver.hpp"

namespace infra {

RCReceiverAdapter::RCReceiverAdapter(gpio_num_t pin) : pin_pwm(pin) {}

void IRAM_ATTR RCReceiverAdapter::gpio_isr_handler(void* arg) {
    auto* receiver = static_cast<RCReceiverAdapter*>(arg);
    uint32_t current_time = (uint32_t)esp_timer_get_time();

    // Read the current state of the pin
    if (gpio_get_level(receiver->pin_pwm) == 1) {
        // Rising Edge: Start timing
        receiver->pulse_start_time = current_time;
    } else {
        // Falling Edge: Calculate pulse width
        if (receiver->pulse_start_time != 0) {
            receiver->pulse_width_us = current_time - receiver->pulse_start_time;
        }
    }
}

bool RCReceiverAdapter::initialize() {
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_ANYEDGE; // Trigger on both High and Low
    io_conf.pin_bit_mask = (1ULL << pin_pwm);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_config(&io_conf);

    // Install ISR service if not already installed by LoRa
    gpio_install_isr_service(0); 
    gpio_isr_handler_add(pin_pwm, gpio_isr_handler, (void*)this);

    return true;
}

bool RCReceiverAdapter::is_recording_active() {
    // Standard RC threshold: 1500us is the middle of the switch
    return (pulse_width_us > 1500);
}

}
