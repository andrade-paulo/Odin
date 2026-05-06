#pragma once
#include "dto_flight_data.hpp"

namespace core {

template <typename Derived>  // CRTP
class IMUPort {
public:
    inline void read_sensor_data(FlightDataDTO& out_dto) {
        static_cast<Derived*>(this)->read_sensor_data_impl(out_dto);
    }

    inline bool initialize() {
        return static_cast<Derived*>(this)->initialize_impl();
    }
    
    inline bool is_healthy() {
        return static_cast<Derived*>(this)->is_healthy_impl();
    }
};

}
