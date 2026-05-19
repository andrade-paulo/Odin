set(MAIN_SRCS
	"main.cpp"
	"rtos_tasks/task_lora_rx.cpp"
	"rtos_tasks/task_udp_tx.cpp"
)

idf_component_register(SRCS ${MAIN_SRCS}
					   INCLUDE_DIRS .
					   REQUIRES core_domain infrastructure nvs_flash esp_netif esp_wifi esp_event lwip driver freertos)
