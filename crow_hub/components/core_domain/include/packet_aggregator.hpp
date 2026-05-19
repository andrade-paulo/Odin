// PacketAggregator: central message buffer to prevent interleaving.
#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/message_buffer.h"
#include <cstdint>
#include <cstddef>

class PacketAggregator {
public:
	static constexpr size_t MESSAGE_BUFFER_BYTES = 8192;
	static constexpr size_t MAX_PACKET_BYTES = 2048;

	PacketAggregator();

	bool push_packet(const uint8_t* data, size_t len);

	// Blocking pop; copies up to max_len into buffer and returns actual bytes.
	size_t pop_packet(uint8_t* buffer, size_t max_len);

private:
	MessageBufferHandle_t message_buffer_{nullptr};
	StaticMessageBuffer_t message_buffer_storage_{};
	uint8_t message_buffer_storage_bytes_[MESSAGE_BUFFER_BYTES]{};
};

PacketAggregator& shared_packet_aggregator();

