#include <cstdint>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_timer.h"
#include "esp_log.h"

// Correctly resolved paths based on CMake exports
#include "adapter_mpu6050.hpp"
#include "telemetry_scheduler.hpp"
#include "adapter_mavlink_radio.hpp"
#include "dto_flight_data.hpp"
#include "adapter_rc_receiver.hpp"
#include "adapter_buzzer.hpp"
#include "task_params.hpp"


extern QueueHandle_t buzzer_queue;
extern QueueHandle_t sd_data_queue;

void vTaskAcquisition(void *pvParameters) {
    // Unpack the injected dependencies
    auto* params = static_cast<AcqTaskParams*>(pvParameters);
    auto* imu = params->imu;
    auto* scheduler = params->scheduler;
    auto* rc_switch = params->rc_switch;
    
    core::FlightDataDTO current_dto;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(10);  // 10ms = 1 tick at 100Hz 

    while (1) {
        bool is_recording = rc_switch->is_recording_active();
        
        // Debug: log recording status periodically
        static uint32_t last_log = 0;
        uint32_t now = (uint32_t)(esp_timer_get_time() / 1000);
        if (now - last_log > 5000) {  // Log every 5 seconds
            ESP_LOGI("ACQ_TASK", "Recording: %s", is_recording ? "ON" : "OFF");
            last_log = now;
        }

        // Edge Detection: Has the switch state changed since the last loop?
        static bool was_recording = false; 
        if (is_recording != was_recording) {
            infra::BuzzerCommand cmd;
            if (is_recording) {
                // START: 1 short beep
                cmd = {1, 100, 100}; 
            } else {
                // STOP: 2 short beeps
                cmd = {2, 100, 100}; 
            }
            // Drop it in the queue without blocking the RTOS (timeout = 0)
            xQueueSend(buzzer_queue, &cmd, 0);
            was_recording = is_recording;
        }

        if (!is_recording) {
            vTaskDelay(pdMS_TO_TICKS(100));
            xLastWakeTime = xTaskGetTickCount(); 
            continue; 
        }

        current_dto.timestamp_ms = (uint32_t)(esp_timer_get_time() / 1000); 
        
        imu->read_sensor_data(current_dto);
        
        xQueueSend(sd_data_queue, &current_dto, 0);
        scheduler->process_frame(current_dto);

        vTaskDelayUntil(&xLastWakeTime, xFrequency); 
    }
}
