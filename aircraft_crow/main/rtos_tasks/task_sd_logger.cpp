#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "dto_flight_data.hpp"

#include <cstdio>
#include <cstring>
#include "esp_log.h"
#include "esp_timer.h"

static const char* TAG = "SD_TASK";

// External queue handle created in main.cpp
extern QueueHandle_t sd_data_queue; 

// 4KB Buffer to prevent thrashing the SD card
#define SD_WRITE_BUFFER_SIZE 4096 
static char write_buffer[SD_WRITE_BUFFER_SIZE];

void vTaskSDLogger(void *pvParameters) {
    FILE* f = nullptr;
    const char* filepath = "/sdcard/flight_log.csv";
    
    // Lambda to safely open/reopen the file
    auto open_file = [&]() -> bool {
        if (f) {
            fflush(f);
            fclose(f);
            f = nullptr;
        }
        f = fopen(filepath, "a");
        if (!f) {
            ESP_LOGE(TAG, "Failed to open %s", filepath);
            return false;
        }
        return true;
    };
    
    if (!open_file()) {
        vTaskDelete(NULL);
    }

    // Write CSV Header
    fprintf(f, "Timestamp_ms,Accel_X,Accel_Y,Accel_Z,Gyro_X,Gyro_Y,Gyro_Z,Status\n");
    fflush(f);

    core::FlightDataDTO incoming_dto;
    size_t buffer_pos = 0;
    char line_buffer[128];
    const TickType_t xQueueTimeout = pdMS_TO_TICKS(1000);
    uint32_t last_reopen = esp_timer_get_time() / 1000;

    while (1) {
        if (xQueueReceive(sd_data_queue, &incoming_dto, xQueueTimeout) == pdPASS) {
            int line_len = snprintf(line_buffer, sizeof(line_buffer), 
                "%lu,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%u\n",
                incoming_dto.timestamp_ms,
                incoming_dto.accel_x, incoming_dto.accel_y, incoming_dto.accel_z,
                incoming_dto.gyro_x, incoming_dto.gyro_y, incoming_dto.gyro_z,
                incoming_dto.system_status_mask
            );

            if (line_len > 0 && line_len < sizeof(line_buffer)) {
                if (buffer_pos + line_len >= SD_WRITE_BUFFER_SIZE) {
                    int written = fwrite(write_buffer, 1, buffer_pos, f);
                    if (written < (int)buffer_pos) {
                        ESP_LOGE(TAG, "fwrite failed: wrote %d of %u bytes", written, buffer_pos);
                    }
                    int flush_ret = fflush(f);
                    if (flush_ret != 0) {
                        ESP_LOGE(TAG, "fflush() failed: %d", flush_ret);
                    }
                    buffer_pos = 0; 
                }

                memcpy(&write_buffer[buffer_pos], line_buffer, line_len);
                buffer_pos += line_len;
            }
        } else {
            // Queue timeout: flush buffered data
            if (buffer_pos > 0) {
                ESP_LOGI(TAG, "Queue timeout - flushing %u bytes to SD", buffer_pos);
                int written = fwrite(write_buffer, 1, buffer_pos, f);
                if (written < (int)buffer_pos) {
                    ESP_LOGE(TAG, "fwrite timeout failed: wrote %d of %u bytes", written, buffer_pos);
                }
                int flush_ret = fflush(f);
                if (flush_ret != 0) {
                    ESP_LOGE(TAG, "fflush() failed during timeout: %d", flush_ret);
                } else {
                    ESP_LOGI(TAG, "Data flushed to SD");
                }
                buffer_pos = 0;
            }
        }
        
        // Periodically close and reopen file to ensure FAT commits data to disk
        uint32_t now = esp_timer_get_time() / 1000;
        if (now - last_reopen > 10000) {  // Every 10 seconds
            ESP_LOGI(TAG, "Reopening file to commit data to disk");
            if (!open_file()) {
                ESP_LOGE(TAG, "Failed to reopen file, retrying...");
                vTaskDelay(pdMS_TO_TICKS(1000));
                open_file();
            }
            // Header already written, don't write again
            last_reopen = now;
        }
    }
}
