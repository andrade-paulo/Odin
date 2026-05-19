// UDP socket adapter for broadcasting MAVLink packets
#pragma once

#include <cstddef>
#include <cstdint>

class AdapterUdpSocket {
public:
	AdapterUdpSocket() = default;
	bool init_impl();
	bool send_packet(const uint8_t* data, size_t len);
private:
	int sockfd{-1};
};

