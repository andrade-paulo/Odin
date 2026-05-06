#pragma once
#include "port_imu_crtp.hpp"
#include "dto_flight_data.hpp"
#include "driver/i2c.h" // ESP-IDF classic I2C driver

namespace infra {

class MPU6050_Adapter : public core::IMUPort<MPU6050_Adapter> {
private:
    i2c_port_t i2c_port;
    uint8_t device_address;
    bool initialized_flag = false;
    
    // Conversion factors for default configuration (±2g, ±250 deg/s)
    static constexpr float ACCEL_SCALE = 9.81f / 16384.0f; // Convert to m/s^2
    static constexpr float GYRO_SCALE = (3.14159f / 180.0f) / 131.0f; // Convert to rad/s

public:
    MPU6050_Adapter(i2c_port_t port, uint8_t addr = 0x68);

    // CRTP Implementations mandated by the base class
    bool initialize_impl();
    bool is_healthy_impl();
    void read_sensor_data_impl(core::FlightDataDTO& out_dto);

    bool is_initialized() const { return initialized_flag; }
};

}
