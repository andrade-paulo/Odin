#include "mavlink_core.h"

int decode_uav_telemetry(const uint8_t* in_buffer, size_t in_len, mavlink_message_t* out_msg) {
    if (in_len < sizeof(uint8_t)) return 0; 

    telemetry_dto_packed_t* dto = (telemetry_dto_packed_t*) in_buffer;

    // Hardcoded System ID do UAV (1) e Component ID (1)
    switch (dto->type) {
        case 0: // IMU
            mavlink_msg_crow_imu_pack(1, 1, out_msg, dto->payload.imu.timestamp_ms,
                dto->payload.imu.accel[0], dto->payload.imu.accel[1], dto->payload.imu.accel[2],
                dto->payload.imu.gyro[0], dto->payload.imu.gyro[1], dto->payload.imu.gyro[2],
                dto->payload.imu.mag[0], dto->payload.imu.mag[1], dto->payload.imu.mag[2]);
            return 1;
        case 1: // BARO
            mavlink_msg_crow_baro_pack(1, 1, out_msg, dto->payload.baro.timestamp_ms,
                dto->payload.baro.pressure_delta, dto->payload.baro.temperature);
            return 1;
        case 2: // GPS
            mavlink_msg_crow_gps_pack(1, 1, out_msg, dto->payload.gps.timestamp_ms,
                dto->payload.gps.latitude, dto->payload.gps.longitude, 
                dto->payload.gps.altitude_msl, dto->payload.gps.ground_speed_ms, 
                (uint8_t)dto->payload.gps.satellites);
            return 1;
        case 3: // COMMAND
            mavlink_msg_crow_command_pack(1, 1, out_msg, 0, (uint8_t)dto->payload.command.command_value);
            return 1;
        default:
            return 0;
    }
}