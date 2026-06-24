#pragma once
#include <stdint.h>
#include <stddef.h>
#include "generated/crow_telemetry/mavlink.h"

// DTO empacotado para casar com a memória do UAV
typedef struct __attribute__((packed)) {
    uint8_t type; 
    union {
        struct __attribute__((packed)) {
            uint32_t timestamp_ms;
            int16_t accel[3];
            int16_t gyro[3];
            int16_t mag[3];
        } imu;
        struct __attribute__((packed)) {
            uint32_t timestamp_ms;
            int16_t pressure_delta;
            int16_t temperature;
        } baro;
        struct __attribute__((packed)) {
            uint32_t timestamp_ms;
            int satellites; // Ajuste se alterou para uint8_t no TX
            int32_t latitude;
            int32_t longitude;
            uint16_t altitude_msl;
            uint16_t ground_speed_ms;
        } gps;
        struct __attribute__((packed)) {
            uint32_t command_value; // Ajuste baseado no seu enum no TX
        } command;
    } payload;
} telemetry_dto_packed_t;

int decode_uav_telemetry(const uint8_t* in_buffer, size_t in_len, mavlink_message_t* out_msg);