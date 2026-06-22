#pragma once

#include <cstdint>
#include <cstddef>
#include "telemetry_dto.hpp"

class MavlinkWrapper {
public:
    MavlinkWrapper(uint8_t system_id, uint8_t component_id);

    // Converte o TelemetryDTO para um frame binário MAVLink v2
    size_t serialize(const TelemetryDTO& dto, uint8_t* tx_buffer, size_t max_length);

private:
    uint8_t _system_id;
    uint8_t _component_id;

    // Métodos internos de mapeamento 1-para-1 (Sem alocação dinâmica)
    size_t pack_imu(const ImuPayload& imu, uint8_t* buffer, size_t max_length);
    size_t pack_barometer(const BarometerPayload& baro, uint8_t* buffer, size_t max_length);
    size_t pack_gps(const GpsPayload& gps, uint8_t* buffer, size_t max_length);
    size_t pack_command(CommandType cmd, uint8_t* buffer, size_t max_length);
};