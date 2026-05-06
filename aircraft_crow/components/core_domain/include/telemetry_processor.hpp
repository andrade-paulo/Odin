#pragma once
#include "port_imu_crtp.hpp"
#include "dto_flight_data.hpp"

namespace core {

template <typename IMUAdapter>
class TelemetryProcessor {
private:
    IMUAdapter& imu_sensor;
    FlightDataDTO current_state;

public:
    // Dependency Injection
    explicit TelemetryProcessor(IMUAdapter& imu) : imu_sensor(imu) {
        current_state = {0};
    }

    // This is the function the FreeRTOS Core 1 task will run in a tight loop
    void execute_cycle(uint32_t current_time_ms) {
        current_state.timestamp_ms = current_time_ms;
        
        imu_sensor.read_sensor_data(current_state);

        if (!imu_sensor.is_healthy()) {
            current_state.system_status_mask &= ~(1 << 0); 
        } else {
            current_state.system_status_mask |= (1 << 0);
        }

        // Next step: push 'current_state' to the serializer/RTOS buffer
    }

    const FlightDataDTO& get_current_state() const {
        return current_state;
    }
};

}
