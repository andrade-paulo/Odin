#include "adapter_buzzer.hpp"

namespace infra {

BuzzerAdapter::BuzzerAdapter(gpio_num_t buzzer_pin) : pin(buzzer_pin) {}

bool BuzzerAdapter::initialize() {
    // 1. Configure the Timer (2000 Hz)
    ledc_timer_config_t ledc_timer = {};
    ledc_timer.speed_mode      = LEDC_LOW_SPEED_MODE;
    ledc_timer.timer_num       = LEDC_TIMER_0;
    ledc_timer.duty_resolution = LEDC_TIMER_10_BIT;
    ledc_timer.freq_hz         = 2000;
    ledc_timer.clk_cfg         = LEDC_AUTO_CLK;
    ledc_timer_config(&ledc_timer);

    // 2. Configure the Channel mapping to the GPIO pin
    ledc_channel_config_t ledc_channel = {};
    ledc_channel.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_channel.channel    = LEDC_CHANNEL_0;
    ledc_channel.timer_sel  = LEDC_TIMER_0;
    ledc_channel.intr_type  = LEDC_INTR_DISABLE;
    ledc_channel.gpio_num   = pin;
    ledc_channel.duty       = 0; // Start muted
    ledc_channel.hpoint     = 0;
    ledc_channel_config(&ledc_channel);

    return true;
}

void BuzzerAdapter::set_state(bool on) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, on ? 512 : 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

}
