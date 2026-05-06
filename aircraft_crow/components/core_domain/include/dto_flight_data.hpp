#pragma once
#include <cstdint>

namespace core {

struct FlightDataDTO {
    uint32_t timestamp_ms;
    
    // Using standard SI units: m/s^2 for accel, rad/s for gyro
    float accel_x;
    float accel_y;
    float accel_z;
    
    float gyro_x;
    float gyro_y;
    float gyro_z;

    // Bitmask for system states (e.g., bit 0: IMU healthy, bit 1: SD logging)
    uint8_t system_status_mask; 
};

}
