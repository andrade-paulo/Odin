#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <cstdio>
#include <ctime>
#include <cerrno>

#include "adapter_rc_receiver.hpp"

static const char* TAG = "PWM_LOGGER";

void vTaskPWMLogger(void* pvParameters) {
    auto* rc = static_cast<infra::RCReceiverAdapter*>(pvParameters);
    if (!rc) {
        ESP_LOGE(TAG, "No RC adapter provided");
        vTaskDelete(NULL);
    }

    // Optional: open a log on SD if available
    FILE* sd_f = fopen("/sdcard/pwm_log.csv", "a");
    if (!sd_f) {
        ESP_LOGE(TAG, "Failed to open /sdcard/pwm_log.csv for writing (errno=%d)", errno);
    } else {
        int ret = fprintf(sd_f, "timestamp_ms,pulse_us\n");
        ESP_LOGI(TAG, "Wrote PWM CSV header: %d bytes", ret);
        int flush_ret = fflush(sd_f);
        if (flush_ret != 0) {
            ESP_LOGE(TAG, "fflush header failed: %d (errno=%d)", flush_ret, errno);
        } else {
            ESP_LOGI(TAG, "PWM CSV header flushed successfully");
        }
    }

    const TickType_t xFrequency = pdMS_TO_TICKS(200);  // 200ms = 5 Hz sampling
    uint32_t last_flush = esp_timer_get_time() / 1000;

    while (1) {
        uint32_t pw = rc->get_pulse_width_us();
        uint32_t ts = (uint32_t)(esp_timer_get_time() / 1000);
        ESP_LOGI(TAG, "PWM: %u us @ %u ms", pw, ts);
        if (sd_f) {
            int ret = fprintf(sd_f, "%lu,%lu\n", (unsigned long)ts, (unsigned long)pw);
            if (ret <= 0) {
                ESP_LOGE(TAG, "fprintf failed: %d (errno=%d)", ret, errno);
            }
        }
        
        // Flush every 2 seconds to ensure data survives power loss
        uint32_t now = esp_timer_get_time() / 1000;
        if (now - last_flush > 2000) {
            if (sd_f) {
                int flush_ret = fflush(sd_f);
                if (flush_ret != 0) {
                    ESP_LOGE(TAG, "fflush data failed: %d (errno=%d)", flush_ret, errno);
                } else {
                    ESP_LOGI(TAG, "PWM data flushed to SD");
                }
            }
            last_flush = now;
        }
        
        vTaskDelay(xFrequency);
    }
}
