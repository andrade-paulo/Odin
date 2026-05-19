#include "adapter_sd_spi.hpp"
#include "esp_log.h"
#include "esp_vfs_fat.h"

static const char* TAG = "SD_SPI_ADAPTER";

namespace infra {

SDCardSPIAdapter::SDCardSPIAdapter(int mosi, int miso, int clk, int cs, spi_host_device_t host, const char* mount_pt)
    : pin_mosi(mosi), pin_miso(miso), pin_clk(clk), pin_cs(cs), 
    mount_point(mount_pt), is_mounted(false), card_handle(nullptr), spi_host_id(host) {}

SDCardSPIAdapter::~SDCardSPIAdapter() {
    if (is_mounted) {
        unmount();
    }
}

bool SDCardSPIAdapter::mount() {
    if (is_mounted) return true;

    esp_err_t ret;

    // 1. Define the SPI host and frequency.
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = spi_host_id;
    host.max_freq_khz = 20000; // 20 MHz is stable for most breadboard/wire setups. Push to 40MHz on custom PCBs.

    // 2. Configure the SPI Bus
    spi_bus_config_t bus_cfg = {};
    bus_cfg.mosi_io_num = pin_mosi;
    bus_cfg.miso_io_num = pin_miso;
    bus_cfg.sclk_io_num = pin_clk;
    bus_cfg.quadwp_io_num = -1;
    bus_cfg.quadhd_io_num = -1;
    bus_cfg.max_transfer_sz = 4000; // Standard 4KB transfer size matches FAT allocation

    // Initialize SPI bus. If already initialized by another sensor, this returns ESP_ERR_INVALID_STATE (which is fine)
    ret = spi_bus_initialize(spi_host_id, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Failed to initialize SPI bus. Error: %s", esp_err_to_name(ret));
        return false;
    }

    // 3. Configure the SD SPI Device Slot
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = (gpio_num_t)pin_cs;
    slot_config.host_id = spi_host_id;

    // 4. Configure FAT Mount Parameters
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {};
    mount_config.format_if_mount_failed = false; // Do not auto-format. If it fails, the pilot needs to know.
    mount_config.max_files = 5;
    mount_config.allocation_unit_size = 16 * 1024;

    // 5. Mount the Filesystem
    ESP_LOGI(TAG, "Mounting filesystem at %s", mount_point);
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card_handle);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. Is card formatted as FAT32?");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). Check lines/pull-ups.", esp_err_to_name(ret));
        }
        return false;
    }

    // Success. Print card specifications to serial for debug
    sdmmc_card_print_info(stdout, card_handle);
    is_mounted = true;
    return true;
}

void SDCardSPIAdapter::unmount() {
    if (!is_mounted) return;

    // Unmount partition and disable SPI peripheral
    esp_vfs_fat_sdcard_unmount(mount_point, card_handle);
    ESP_LOGI(TAG, "Card unmounted successfully.");
    
    // Note: If you share this SPI bus with an IMU or LoRa, DO NOT call spi_bus_free here.
    // We assume the bus stays alive for the duration of the system power-cycle.
    
    is_mounted = false;
    card_handle = nullptr;
}

} // namespace infra
