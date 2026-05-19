// LoRa UART adapter implementation
#include "adapter_lora_uart.hpp"
#include "esp_log.h"
#include <driver/uart.h>
#include <string.h>
#include "../../../main/board_pinout_config.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "AdapterLoraUart";
#define LORA_UART_NUM UART_NUM_1
#define LORA_UART_BUF_SIZE 2048

bool AdapterLoraUart::init_impl()
{
	uart_config_t uart_config = {};
	uart_config.baud_rate = 57600;
	uart_config.data_bits = UART_DATA_8_BITS;
	uart_config.parity = UART_PARITY_DISABLE;
	uart_config.stop_bits = UART_STOP_BITS_1;
	uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
	esp_err_t err = uart_param_config(LORA_UART_NUM, &uart_config);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "uart_param_config failed: %d", err);
		return false;
	}
	err = uart_driver_install(LORA_UART_NUM, LORA_UART_BUF_SIZE * 2, 0, 0, NULL, 0);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "uart_driver_install failed: %d", err);
		return false;
	}
	// Configure pins from main/board_pinout_config.hpp
	err = uart_set_pin(LORA_UART_NUM, LORA_UART_TX_PIN, LORA_UART_RX_PIN, LORA_UART_RTS_PIN, LORA_UART_CTS_PIN);
	if (err != ESP_OK) {
		ESP_LOGW(TAG, "uart_set_pin failed: %d (check pin defines)", err);
	}
	ESP_LOGI(TAG, "LoRa UART initialized");
	return true;
}

size_t AdapterLoraUart::read_bytes_impl(uint8_t* buffer, size_t max_len)
{
	// Wait up to 100ms for data so reads are not purely non-blocking
	int r = uart_read_bytes(LORA_UART_NUM, buffer, max_len, pdMS_TO_TICKS(100));
	if (r < 0) return 0;
	return static_cast<size_t>(r);
}

size_t AdapterLoraUart::write_bytes_impl(const uint8_t* data, size_t len)
{
	if (data == nullptr || len == 0) return 0;
	int w = uart_write_bytes(LORA_UART_NUM, reinterpret_cast<const char*>(data), len);
	if (w < 0) return 0;
	return static_cast<size_t>(w);
}

