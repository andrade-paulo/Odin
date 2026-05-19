// UDP broadcaster implementation
#include "adapter_udp_socket.hpp"
#include "esp_log.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

static const char* TAG = "AdapterUdpSocket";

bool AdapterUdpSocket::init_impl()
{
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (sockfd < 0) {
		esp_log_write(ESP_LOG_ERROR, TAG, "socket failed");
		return false;
	}
	int broadcast = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) != 0) {
		esp_log_write(ESP_LOG_WARN, TAG, "SO_BROADCAST enable failed");
	}
	esp_log_write(ESP_LOG_INFO, TAG, "UDP socket ready on port 14550");
	return true;
}

bool AdapterUdpSocket::send_packet(const uint8_t* data, size_t len)
{
	if (sockfd < 0) return false;
	struct sockaddr_in dest = {};
	dest.sin_family = AF_INET;
	dest.sin_port = htons(14550);
	dest.sin_addr.s_addr = inet_addr("255.255.255.255");

	ssize_t r = sendto(sockfd, data, len, 0, (struct sockaddr*)&dest, sizeof(dest));
	if (r < 0) {
		esp_log_write(ESP_LOG_ERROR, TAG, "sendto failed");
	}
	return r == (ssize_t)len;
}
