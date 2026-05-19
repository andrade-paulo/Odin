set(INFRA_SRCS
	"src/adapter_wifi_ap.cpp"
	"src/adapter_udp_socket.cpp"
	"src/adapter_rf_spi.cpp"
)

idf_component_register(SRCS ${INFRA_SRCS}
					   INCLUDE_DIRS include
					   REQUIRES esp_driver_spi esp_driver_gpio esp_netif esp_wifi esp_event lwip freertos)
