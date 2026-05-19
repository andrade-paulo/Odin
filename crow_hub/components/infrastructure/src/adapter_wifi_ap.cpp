// Simple Wi-Fi AP initialization using esp-idf APIs
#include "adapter_wifi_ap.hpp"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include <string.h>

static const char* TAG = "AdapterWifiAp";

bool AdapterWifiAp::init_impl()
{
	esp_err_t err = esp_netif_init();
	if (err != ESP_OK) {
		esp_log_write(ESP_LOG_ERROR, TAG, "esp_netif_init failed");
		return false;
	}
	err = esp_event_loop_create_default();
	if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
		esp_log_write(ESP_LOG_ERROR, TAG, "esp_event_loop_create_default failed");
		return false;
	}

	esp_netif_create_default_wifi_ap();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	err = esp_wifi_init(&cfg);
	if (err != ESP_OK) {
		esp_log_write(ESP_LOG_ERROR, TAG, "esp_wifi_init failed");
		return false;
	}

	wifi_config_t wifi_config = {};
	const char* ssid = "Telemetry_Hub";
	strncpy((char*)wifi_config.ap.ssid, ssid, sizeof(wifi_config.ap.ssid));
	wifi_config.ap.ssid_len = strlen(ssid);
	wifi_config.ap.max_connection = 4;
	wifi_config.ap.authmode = WIFI_AUTH_OPEN;

	err = esp_wifi_set_mode(WIFI_MODE_AP);
	if (err != ESP_OK) return false;
	err = esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
	if (err != ESP_OK) return false;
	err = esp_wifi_start();
	if (err != ESP_OK) return false;

	esp_log_write(ESP_LOG_INFO, TAG, "Started AP SSID: %s", ssid);
	return true;
}

