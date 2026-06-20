#pragma once

#include <cstdint>


struct ImuPayload {
    uint32_t timestamp_ms;

    // Acceleration using m/s^2
    float linear_acceleration_x;
    float linear_acceleration_y;
    float linear_acceleration_z;
    
    // Rotation speed using °/s
    float rotation_speed_x;
    float rotation_speed_y;
    float rotation_speed_z;
};

struct BarometerPayload {
    uint32_t timestamp_ms;

    float pressure_pa;
    float temperature_c;
};

struct GpsPayload {
    uint32_t timestamp_ms;

    // Degrees
    float latitude;
    float longitude;
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