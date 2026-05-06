#pragma once
#include "port_radio_crtp.hpp"
#include "dto_flight_data.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/stream_buffer.h"

#include "custom_telemetry/mavlink.h"

namespace infra {

class MavlinkRadioAdapter : public core::RadioPort<MavlinkRadioAdapter> {
private:
    StreamBufferHandle_t tx_stream_buffer;
    uint8_t system_id;
    uint8_t component_id;

    // Helper to push bytes to the FreeRTOS Stream Buffer
    void push_to_radio_task(mavlink_message_t* msg) {
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t len = mavlink_msg_to_send_buffer(buffer, msg);
        
        // Push raw bytes. Do not block if buffer is full (drop packet to maintain realtime execution)
        xStreamBufferSend(tx_stream_buffer, buffer, len, 0);
    }

public:
    MavlinkRadioAdapter(StreamBufferHandle_t stream, uint8_t sys_id = 1, uint8_t comp_id = 1)
        : tx_stream_buffer(stream), system_id(sys_id), component_id(comp_id) {}

    // CRTP Implementation mandated by the base class
    inline void dispatch_telemetry_impl(const core::FlightDataDTO& data, core::TelemetrySlot slot) {
        mavlink_message_t msg;

        switch (slot) {
            case core::TelemetrySlot::ATTITUDE_HIGH_RATE: {
                // Quantization: Multiply floats by 1000 to send as int16_t milli-G / milli-rad
                // This compresses the payload to fit LoRa MTU limits.
                int16_t ax_mg = static_cast<int16_t>(data.accel_x * 1000.0f);
                int16_t ay_mg = static_cast<int16_t>(data.accel_y * 1000.0f);
                int16_t az_mg = static_cast<int16_t>(data.accel_z * 1000.0f);
                int16_t gx_mr = static_cast<int16_t>(data.gyro_x * 1000.0f);
                int16_t gy_mr = static_cast<int16_t>(data.gyro_y * 1000.0f);
                int16_t gz_mr = static_cast<int16_t>(data.gyro_z * 1000.0f);

                mavlink_msg_custom_attitude_pack(system_id, component_id, &msg, 
                                                data.timestamp_ms, 
                                                ax_mg, ay_mg, az_mg, 
                                                gx_mr, gy_mr, gz_mr);
                push_to_radio_task(&msg);
                break;
            }
            case core::TelemetrySlot::NAVIGATION_LOW_RATE: {
                // Future expansion: Pack GPS/Barometer data here
                break;
            }
            case core::TelemetrySlot::SYSTEM_HEALTH_SLOW: {
                // Pass 0 for vbat_mv since we aren't reading an ADC yet
                mavlink_msg_custom_health_pack(system_id, component_id, &msg,
                                              data.timestamp_ms,
                                              0, // vbat_mv (Dummy data)
                                              data.system_status_mask);
                push_to_radio_task(&msg);
                break;
            }
        }
    }
};

}
