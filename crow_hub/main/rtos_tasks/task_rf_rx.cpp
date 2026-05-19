// RF RX producer task
#include "../../components/infrastructure/include/adapter_rf_spi.hpp"
#include "../../components/core_domain/include/packet_aggregator.hpp"
#include "../task_params.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern StackType_t rf_task_stack[];
extern StaticTask_t rf_task_tcb;

static PacketAggregator aggregator;

void task_rf_rx(void* pv)
{
	AdapterRfSpi adapter;
	adapter.init_impl();
	uint8_t buf[1024];
	for (;;){
		size_t n = adapter.read_bytes_impl(buf, sizeof(buf));
		if (n>0){
			aggregator.push_packet(buf, n);
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}
