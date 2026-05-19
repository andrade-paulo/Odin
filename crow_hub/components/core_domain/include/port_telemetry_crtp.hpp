// Port telemetry base using CRTP. No virtual methods, static polymorphism.
#pragma once

#include <cstddef>
#include <cstdint>

template <typename Derived>
class PortTelemetryCrtp {
public:
	bool init()
	{
		return static_cast<Derived*>(this)->init_impl();
	}

	// Non-blocking read: returns number of bytes read up to max_len
	size_t read_bytes(uint8_t* buffer, size_t max_len)
	{
		return static_cast<Derived*>(this)->read_bytes_impl(buffer, max_len);
	}
};

