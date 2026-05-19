// main entry: initialize NVS, Wi-Fi AP, create tasks with static stacks
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "../components/infrastructure/include/adapter_wifi_ap.hpp"
#include "../components/infrastructure/include/adapter_udp_socket.hpp"
#include "task_params.hpp"

static const char* TAG = "main";

// Define the static stacks and TCBs declared in task_params.hpp
StackType_t lora_task_stack[LORA_TASK_STACK_WORDS];
StaticTask_t lora_task_tcb;

StackType_t udp_task_stack[UDP_TASK_STACK_WORDS];
StaticTask_t udp_task_tcb;

extern void task_lora_rx(void* pv);
extern void task_udp_tx(void* pv);

extern "C" void app_main()
{
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		nvs_flash_erase();
		nvs_flash_init();
	}

	AdapterWifiAp wifi;
	if (!wifi.init_impl()) {
		esp_log_write(ESP_LOG_ERROR, TAG, "Wi-Fi init failed");
	}

	// Create tasks pinned to cores with static stacks
	xTaskCreateStaticPinnedToCore(task_lora_rx, "lora_rx", LORA_TASK_STACK_WORDS, NULL, LORA_TASK_PRIORITY, lora_task_stack, &lora_task_tcb, 1);
	xTaskCreateStaticPinnedToCore(task_udp_tx, "udp_tx", UDP_TASK_STACK_WORDS, NULL, UDP_TASK_PRIORITY, udp_task_stack, &udp_task_tcb, 0);

	esp_log_write(ESP_LOG_INFO, TAG, "App initialization complete");
}

