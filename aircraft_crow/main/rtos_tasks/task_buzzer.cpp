#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "adapter_buzzer.hpp"

// External reference to the queue we will create in main.cpp
extern QueueHandle_t buzzer_queue;

void vTaskBuzzer(void *pvParameters) {
    auto* buzzer = static_cast<infra::BuzzerAdapter*>(pvParameters);
    infra::BuzzerCommand cmd;

    while (1) {
        // Block indefinitely until someone sends a command
        if (xQueueReceive(buzzer_queue, &cmd, portMAX_DELAY)) {
            
            for (int i = 0; i < cmd.beeps; i++) {
                buzzer->set_state(true);
                vTaskDelay(pdMS_TO_TICKS(cmd.duration_on_ms)); // Blocks safely!
                
                buzzer->set_state(false);
                vTaskDelay(pdMS_TO_TICKS(cmd.duration_off_ms));
            }
        }
    }
}
