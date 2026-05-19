#include "freertos/FreeRTOS.h"
#include "freertos/message_buffer.h"
#include "adapter_sx1276.hpp"
#include "esp_log.h"

static const char* TAG = "LORA_TASK";

// External message buffer created in main.cpp
extern MessageBufferHandle_t lora_tx_stream;

void vTaskLoRaTX(void *pvParameters) {
    auto* radio_phy = static_cast<infra::SX1276_Adapter*>(pvParameters);

    // The max MAVLink v2 packet size is ~280 bytes, but SX1276 MTU is 255.
    // Our optimized MAVLink payload must stay well under 255.
    uint8_t rx_buffer[255]; 

    while (1) {
        // 1. Block indefinitely waiting for a full MAVLink packet.
        size_t received_bytes = xMessageBufferReceive(lora_tx_stream, rx_buffer, sizeof(rx_buffer), portMAX_DELAY);

        if (received_bytes > 0) {
            // 2. Hand off to the PHY layer. 
            // This function handles the SPI load and blocks the task (yielding CPU) 
            // until the hardware DIO0 interrupt fires.
            bool success = radio_phy->transmit_packet_blocking(rx_buffer, received_bytes);
            
            if (!success) {
                ESP_LOGW(TAG, "Packet drop or TX failure");
            } else {
                ESP_LOGI(TAG, "Packet sent: %u bytes", (unsigned)received_bytes);
            }
        }
    }
}
