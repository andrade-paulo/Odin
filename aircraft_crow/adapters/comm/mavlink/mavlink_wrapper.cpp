#include "mavlink_wrapper.hpp"

#include "esp_timer.h"
#include "generated/crow_telemetry/mavlink.h"


MavlinkWrapper::MavlinkWrapper(uint8_t system_id, uint8_t component_id)
    : _system_id(system_id), _component_id(component_id) 
{
}

size_t MavlinkWrapper::serialize(const TelemetryDTO& dto, uint8_t* tx_buffer, size_t max_length) {
    if (tx_buffer == nullptr || max_length < MAVLINK_MAX_PACKET_LEN) {  // Impede buffer overflow
        return 0; 
    }

    switch (dto.type) {
        case MessageType::IMU:
            return pack_imu(dto.payload.imu, tx_buffer, max_length);
        case MessageType::BARO:
            return pack_barometer(dto.payload.barometer, tx_buffer, max_length);
        case MessageType::GPS:
            return pack_gps(dto.payload.gps, tx_buffer, max_length);
        case MessageType::COMMAND:
            return pack_command(dto.payload.command, tx_buffer, max_length);
        default:
            return 0; // Tipo desconhecido
    }
}

size_t MavlinkWrapper::pack_imu(const ImuPayload& imu, uint8_t* buffer, size_t max_length) {
    mavlink_message_t msg;
    
    // Empacota os inteiros directamente (A quantização já foi feita na imu_task)
    mavlink_msg_crow_imu_pack(
        _system_id, 
        _component_id, 
        &msg,
        imu.timestamp_ms,
        imu.linear_acceleration_x, imu.linear_acceleration_y, imu.linear_acceleration_z,
        imu.rotation_speed_x, imu.rotation_speed_y, imu.rotation_speed_z,
        imu.magnetic_field_x, imu.magnetic_field_y, imu.magnetic_field_z
    );

    // MAVLink v2: Trunca bytes nulos finais, anexa cabeçalho, calcula CRC
    return mavlink_msg_to_send_buffer(buffer, &msg);
}

size_t MavlinkWrapper::pack_barometer(const BarometerPayload& baro, uint8_t* buffer, size_t max_length) {
    mavlink_message_t msg;
    
    mavlink_msg_crow_baro_pack(
        _system_id, 
        _component_id, 
        &msg,
        baro.timestamp_ms,
        baro.pressure_delta,
        baro.temperature
    );

    return mavlink_msg_to_send_buffer(buffer, &msg);
}

size_t MavlinkWrapper::pack_gps(const GpsPayload& gps, uint8_t* buffer, size_t max_length) {
    mavlink_message_t msg;
    
    mavlink_msg_crow_gps_pack(
        _system_id, 
        _component_id, 
        &msg,
        gps.timestamp_ms,
        gps.latitude,
        gps.longitude,
        gps.altitude_msl,
        gps.ground_speed_ms,
        (uint8_t)gps.satellites
    );

    return mavlink_msg_to_send_buffer(buffer, &msg);
}

size_t MavlinkWrapper::pack_command(CommandType cmd, uint8_t* buffer, size_t max_length) {
    mavlink_message_t msg;
    
    uint8_t cmd_value = 0;
    if (cmd == CommandType::START_RECORDING) cmd_value = 1; // CROW_CMD_START_RECORDING
    else if (cmd == CommandType::STOP_RECORDING) cmd_value = 2; // CROW_CMD_STOP_RECORDING
    
    mavlink_msg_crow_command_pack(
        _system_id, 
        _component_id, 
        &msg,
        esp_timer_get_time() / 1000,
        cmd_value
    );

    return mavlink_msg_to_send_buffer(buffer, &msg);
}