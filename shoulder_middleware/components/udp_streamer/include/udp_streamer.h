#pragma once
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#define UDP_MAX_PACKET_LEN 280

typedef struct {
    uint8_t payload[UDP_MAX_PACKET_LEN];
    uint16_t length;
} udp_packet_t;

void udp_streamer_init(QueueHandle_t tx_queue);