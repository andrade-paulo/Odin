// UDP TX consumer task
#include "../../components/infrastructure/include/adapter_udp_socket.hpp"
#include "../../components/core_domain/include/packet_aggregator.hpp"
#include "../task_params.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <cstring>
#include "esp_log.h"
#include <stdio.h>

static const char* TAG = "task_udp_tx";

extern StackType_t udp_task_stack[];
extern StaticTask_t udp_task_tcb;

void task_udp_tx(void* pv)
{
	AdapterUdpSocket udp;
	if (!udp.init_impl()) {
		ESP_LOGE(TAG, "UDP init failed — terminating UDP task");
		vTaskDelete(nullptr);
		return;
	}
	ESP_LOGI(TAG, "UDP task started");
	static uint8_t buf[2048];
	uint32_t timeout_count = 0;
	for (;;){
		size_t n = shared_packet_aggregator().pop_packet(buf, sizeof(buf));
		if (n>0){
			timeout_count = 0;
			ESP_LOGI(TAG, "Aggregator popped %u bytes for UDP", (unsigned)n);
			bool ok = udp.send_packet(buf, n);
			ESP_LOGI(TAG, "UDP send %u bytes -> %s", (unsigned)n, ok ? "OK" : "FAIL");
		} else {
			++timeout_count;
			if ((timeout_count % 5U) == 0U) {
				ESP_LOGW(TAG, "No packet available after %u timeouts", (unsigned)timeout_count);
			}
			
			// No packet available within timeout — give the scheduler some time so
			// idle tasks can run and the WDT is serviced.
			vTaskDelay(pdMS_TO_TICKS(100));
		}
	}
}
