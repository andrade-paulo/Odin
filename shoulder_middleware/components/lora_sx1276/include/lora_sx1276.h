#pragma once

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/spi_master.h"

#define LORA_MAX_PAYLOAD 256

typedef struct {
    uint8_t payload[LORA_MAX_PAYLOAD];
    uint16_t length;
} lora_rx_packet_t;

typedef struct {
    spi_device_handle_t spi_handle;
    QueueHandle_t rx_queue;
    TaskHandle_t isr_task;
    int cs_pin;
    int rst_pin;
    int irq_pin;
} lora_ctx_t;

esp_err_t lora_init(lora_ctx_t* ctx);