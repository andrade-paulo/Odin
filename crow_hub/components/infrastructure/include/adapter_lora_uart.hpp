// LoRa UART adapter implementing PortTelemetryCrtp interface
#pragma once

#include "../../core_domain/include/port_telemetry_crtp.hpp"
#include <cstddef>
#include <cstdint>

class AdapterLoraUart : public PortTelemetryCrtp<AdapterLoraUart> {
public:
	AdapterLoraUart() = default;
	bool init_impl();
	size_t read_bytes_impl(uint8_t* buffer, size_t max_len);
	size_t write_bytes_impl(const uint8_t* data, size_t len);
};

