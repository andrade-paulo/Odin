#pragma once

#include <cstdint>


struct ImuPayload {
    uint32_t timestamp_ms;

    // Acceleration using m/s^2
    int16_t linear_acceleration_x;
    int16_t linear_acceleration_y;
    int16_t linear_acceleration_z;
    
    // Rotation speed using °/s
    int16_t rotation_speed_x;
    int16_t rotation_speed_y;
    int16_t rotation_speed_z;

    // Magnetometro
    int16_t magnetic_field_x;
    int16_t magnetic_field_y;
    int16_t magnetic_field_z;
};

struct BarometerPayload {
    uint32_t timestamp_ms;

    int16_t pressure_delta;
    int16_t temperature;
};

struct GpsPayload {
    uint32_t timestamp_ms;

    int satellites;
    int32_t latitude;
    int32_t longitude;
    uint16_t altitude_msl;
    uint16_t ground_speed_ms;
};


enum class MessageType { 
    IMU,
    BARO,
    GPS,
    COMMAND
};

enum class CommandType {
    START_RECORDING,
    STOP_RECORDING
};


struct TelemetryDTO {
    MessageType type;

    union {
        ImuPayload imu;
        BarometerPayload barometer;
        GpsPayload gps;
        CommandType command;
    } payload;
};