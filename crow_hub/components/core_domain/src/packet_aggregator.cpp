// Shared packet aggregator instance used by LoRa RX and UDP TX tasks.
#include "../include/packet_aggregator.hpp"
#include <string.h>

PacketAggregator::PacketAggregator()
{
	message_buffer_ = xMessageBufferCreateStatic(
		MESSAGE_BUFFER_BYTES,
		message_buffer_storage_bytes_,
		&message_buffer_storage_);
}

bool PacketAggregator::push_packet(const uint8_t* data, size_t len)
{
	if (message_buffer_ == nullptr || data == nullptr || len == 0 || len > MAX_PACKET_BYTES) {
		return false;
	}

	return xMessageBufferSend(message_buffer_, data, len, 0) == len;
}

size_t PacketAggregator::pop_packet(uint8_t* buffer, size_t max_len)
{
	if (message_buffer_ == nullptr || buffer == nullptr || max_len == 0) {
		return 0;
	}

	const TickType_t kTimeout = pdMS_TO_TICKS(1000);
	size_t received = xMessageBufferReceive(message_buffer_, buffer, max_len, kTimeout);
	if (received == 0) {
		return 0;
	}

	return received;
}

PacketAggregator& shared_packet_aggregator()
{
	static PacketAggregator instance;
	return instance;
}