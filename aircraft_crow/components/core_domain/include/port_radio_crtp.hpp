#pragma once
#include "dto_flight_data.hpp"

namespace core {

enum class TelemetrySlot {
    ATTITUDE_HIGH_RATE, // 10 Hz: Fast dynamic state
    NAVIGATION_LOW_RATE, // 1 Hz: GPS, altitude
    SYSTEM_HEALTH_SLOW   // 0.1 Hz: Battery, temperatures, status
};

template <typename Derived>
class RadioPort {
public:
    // The scheduler calls this. The adapter implements the protocol packing.
    inline void dispatch_telemetry(const FlightDataDTO& current_data, TelemetrySlot slot) {
        static_cast<Derived*>(this)->dispatch_telemetry_impl(current_data, slot);
    }
};

}
