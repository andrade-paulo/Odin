#pragma once

#include "../../core_domain/include/port_telemetry_crtp.hpp"
#include <driver/spi_master.h>
#include <cstdint>
#include <cstddef>

class AdapterRfSpi : public PortTelemetryCrtp<AdapterRfSpi> {
public:
	AdapterRfSpi() = default;
	bool init_impl();
	size_t read_bytes_impl(uint8_t* buffer, size_t max_len);

private:
	bool reset_radio();
	bool configure_radio();
	bool write_register(uint8_t reg, uint8_t value);
	bool read_register(uint8_t reg, uint8_t* value);
	bool burst_read(uint8_t reg, uint8_t* buffer, size_t len);

	spi_device_handle_t spi_device_ = nullptr;
	bool initialized_ = false;
};

