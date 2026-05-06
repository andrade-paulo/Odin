#include "adapter_mpu6050.hpp"
#include "esp_log.h"

static const char* TAG = "MPU6050_ADAPTER";

// MPU6050 Registers
#define MPU6050_REG_ACCEL_XOUT_H 0x3B
#define MPU6050_REG_PWR_MGMT_1   0x6B
#define MPU6050_REG_WHO_AM_I     0x75

namespace infra {

MPU6050_Adapter::MPU6050_Adapter(i2c_port_t port, uint8_t addr) 
    : i2c_port(port), device_address(addr) {}

bool MPU6050_Adapter::initialize_impl() {
    // Wake up the MPU6050 (write 0x00 to PWR_MGMT_1)
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (device_address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, MPU6050_REG_PWR_MGMT_1, true);
    i2c_master_write_byte(cmd, 0x00, true);
    i2c_master_stop(cmd);
    
    esp_err_t ret = i2c_master_cmd_begin(i2c_port, cmd, pdMS_TO_TICKS(10));
    i2c_cmd_link_delete(cmd);
    
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize MPU6050");
        return false;
    }

    this->initialized_flag = true;
    return true;
}

bool MPU6050_Adapter::is_healthy_impl() {
    // Ping the WHO_AM_I register
    uint8_t who_am_i = 0;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (device_address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, MPU6050_REG_WHO_AM_I, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (device_address << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, &who_am_i, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    
    esp_err_t ret = i2c_master_cmd_begin(i2c_port, cmd, pdMS_TO_TICKS(10));
    i2c_cmd_link_delete(cmd);

    // MPU6050 WHO_AM_I default is 0x68
    return (ret == ESP_OK) && (who_am_i == 0x68);
}

void MPU6050_Adapter::read_sensor_data_impl(core::FlightDataDTO& out_dto) {
    uint8_t raw_data[14]; // 6 bytes Accel, 2 bytes Temp, 6 bytes Gyro

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (device_address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, MPU6050_REG_ACCEL_XOUT_H, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (device_address << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, raw_data, 13, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, raw_data + 13, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    
    esp_err_t ret = i2c_master_cmd_begin(i2c_port, cmd, pdMS_TO_TICKS(10));
    i2c_cmd_link_delete(cmd);

    if (ret == ESP_OK) {
        // Bitwise operations to reconstruct 16-bit integers, then cast and scale to SI units
        out_dto.accel_x = (int16_t)((raw_data[0] << 8) | raw_data[1]) * ACCEL_SCALE;
        out_dto.accel_y = (int16_t)((raw_data[2] << 8) | raw_data[3]) * ACCEL_SCALE;
        out_dto.accel_z = (int16_t)((raw_data[4] << 8) | raw_data[5]) * ACCEL_SCALE;

        // Skip temperature (raw_data[6] and [7])

        out_dto.gyro_x = (int16_t)((raw_data[8] << 8) | raw_data[9]) * GYRO_SCALE;
        out_dto.gyro_y = (int16_t)((raw_data[10] << 8) | raw_data[11]) * GYRO_SCALE;
        out_dto.gyro_z = (int16_t)((raw_data[12] << 8) | raw_data[13]) * GYRO_SCALE;
    } else {
        out_dto.accel_x = 0.0f;
        out_dto.accel_y = 0.0f;
        out_dto.accel_z = 0.0f;
        out_dto.gyro_x = 0.0f;
        out_dto.gyro_y = 0.0f;
        out_dto.gyro_z = 0.0f;
    }
}

}
