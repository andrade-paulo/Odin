#include "udp_streamer.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/sockets.h"

static const char *TAG = "UDP_STREAMER";

static void udp_tx_task(void *pvParameters) {
    QueueHandle_t udp_queue = (QueueHandle_t)pvParameters;
    udp_packet_t packet;

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr("192.168.4.2"); // IP Estático esperado da GCS
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(14550);

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) {
        ESP_LOGE(TAG, "Falha ao criar socket");
        vTaskDelete(NULL);
    }

    ESP_LOGI(TAG, "UDP Streamer rodando. Direcionado para 192.168.4.2:14550");

    while (1) {
        if (xQueueReceive(udp_queue, &packet, portMAX_DELAY) == pdTRUE) {
            sendto(sock, packet.payload, packet.length, 0, 
                   (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        }
    }
}

void udp_streamer_init(QueueHandle_t tx_queue) {
    // Inicialização NVS necessária para Wi-Fi
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      nvs_flash_init();
    }

    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();

    // Configura IP estático do Gateway (ESP32)
    esp_netif_ip_info_t ip_info;
    IP4_ADDR(&ip_info.ip, 192, 168, 4, 1);
    IP4_ADDR(&ip_info.gw, 192, 168, 4, 1);
    IP4_ADDR(&ip_info.netmask, 255, 255, 255, 0);
    esp_netif_dhcps_stop(ap_netif);
    esp_netif_set_ip_info(ap_netif, &ip_info);
    esp_netif_dhcps_start(ap_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = "CROW_TELEMETRY",
            .ssid_len = 14,
            .password = "12345678",
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA2_PSK
        },
    };
    
    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    esp_wifi_start();

    xTaskCreatePinnedToCore(udp_tx_task, "udp_tx", 4096, (void*)tx_queue, 3, NULL, 0);
}