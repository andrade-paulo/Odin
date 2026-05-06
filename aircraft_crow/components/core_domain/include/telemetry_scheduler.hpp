#pragma once
#include "port_radio_crtp.hpp"
#include "dto_flight_data.hpp"

namespace core {

template <typename RadioAdapter>
class TelemetryScheduler {
private:
    RadioAdapter& radio;
    
    uint32_t last_attitude_ms;
    uint32_t last_nav_ms;
    uint32_t last_health_ms;

    // Timing thresholds (configurable per airframe)
    static constexpr uint32_t INTERVAL_ATTITUDE_MS = 100;   // 10 Hz
    static constexpr uint32_t INTERVAL_NAV_MS = 1000;       // 1 Hz
    static constexpr uint32_t INTERVAL_HEALTH_MS = 10000;   // 0.1 Hz

public:
    explicit TelemetryScheduler(RadioAdapter& radio_instance) 
        : radio(radio_instance), last_attitude_ms(0), last_nav_ms(0), last_health_ms(0) {}

    // Called at 100Hz or 200Hz by the Core 1 Acquisition Task
    void process_frame(const FlightDataDTO& current_state) {
        uint32_t now = current_state.timestamp_ms;

        if (now - last_attitude_ms >= INTERVAL_ATTITUDE_MS) {
            radio.dispatch_telemetry(current_state, TelemetrySlot::ATTITUDE_HIGH_RATE);
            last_attitude_ms = now;
        }

        if (now - last_nav_ms >= INTERVAL_NAV_MS) {
            radio.dispatch_telemetry(current_state, TelemetrySlot::NAVIGATION_LOW_RATE);
            last_nav_ms = now;
        }

        if (now - last_health_ms >= INTERVAL_HEALTH_MS) {
            radio.dispatch_telemetry(current_state, TelemetrySlot::SYSTEM_HEALTH_SLOW);
            last_health_ms = now;
        }
    }
};

} // namespace core
