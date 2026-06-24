#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "lora_sx1276.h"
#include "mavlink_core.h"
#include "udp_streamer.h"

static const char *TAG = "ORCHESTRATOR";

QueueHandle_t lora_rx_queue;
QueueHandle_t udp_tx_queue;

static lora_ctx_t lora_ctx;

static void orchestrator_task(void *pvParameters) {
    lora_rx_packet_t raw_lora_pkt;
    udp_packet_t final_udp_pkt;

    ESP_LOGI(TAG, "Orquestrador Transparente Rodando.");

    while (1) {
        if (xQueueReceive(lora_rx_queue, &raw_lora_pkt, portMAX_DELAY)) {
            
            if (raw_lora_pkt.length > 0 && raw_lora_pkt.payload[0] == 0xFD) {
                memcpy(final_udp_pkt.payload, raw_lora_pkt.payload, raw_lora_pkt.length);
                final_udp_pkt.length = raw_lora_pkt.length;
                
                xQueueSend(udp_tx_queue, &final_udp_pkt, 0);
            }
        }
    }
}

void app_main(void) {
    lora_rx_queue = xQueueCreate(20, sizeof(lora_rx_packet_t));
    udp_tx_queue  = xQueueCreate(20, sizeof(udp_packet_t));

    lora_ctx.cs_pin = 5;
    lora_ctx.rst_pin = 14;
    lora_ctx.irq_pin = 34; 
    lora_ctx.rx_queue = lora_rx_queue;

    udp_streamer_init(udp_tx_queue);
    
    lora_init(&lora_ctx);

    xTaskCreatePinnedToCore(orchestrator_task, "orchestrator", 4096, NULL, 4, NULL, 1);
}