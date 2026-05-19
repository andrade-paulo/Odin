// LoRa RX producer task
#include "../../components/infrastructure/include/adapter_rf_spi.hpp"
#include "../../components/core_domain/include/packet_aggregator.hpp"
#include "../task_params.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <cstring>
#include "esp_log.h"

static const char* TAG = "task_lora_rx";

extern StackType_t lora_task_stack[];
extern StaticTask_t lora_task_tcb;

void task_lora_rx(void* pv)
{
	AdapterRfSpi adapter;
	if (!adapter.init_impl()) {
		ESP_LOGE(TAG, "LoRa radio init failed");
		vTaskDelete(nullptr);
		return;
	}
	uint8_t buf[1024];
	for (;;){
		size_t n = adapter.read_bytes_impl(buf, sizeof(buf));
		if (n>0){
			// Log receipt and a short hex preview for debugging
			size_t preview = n < 8 ? n : 8;
			char preview_hex[8 * 2 + 1] = {0};
			for (size_t i = 0; i < preview; ++i) {
				sprintf(&preview_hex[i*2], "%02X", buf[i]);
			}
			ESP_LOGI(TAG, "LoRa received %u bytes, preview=%s", (unsigned)n, preview_hex);
			// In a real build use mavlink_parse_char; here we forward full read as packet
			bool queued = shared_packet_aggregator().push_packet(buf, n);
			ESP_LOGI(TAG, "Pushed %u bytes to aggregator -> %s", (unsigned)n, queued ? "OK" : "FAIL");
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

