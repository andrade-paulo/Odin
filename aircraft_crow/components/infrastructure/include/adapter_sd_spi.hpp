#pragma once
#include "esp_err.h"
#include "sdmmc_cmd.h"
#include "driver/spi_master.h"

namespace infra {

class SDCardSPIAdapter {
private:
    int pin_mosi;
    int pin_miso;
    int pin_clk;
    int pin_cs;
    const char* mount_point;
    
    bool is_mounted;
    sdmmc_card_t* card_handle;
    spi_host_device_t spi_host_id;

public:
    // Pass standard SPI pins. mount_pt defaults to "/sdcard"
    SDCardSPIAdapter(int mosi, int miso, int clk, int cs, const char* mount_pt = "/sdcard");
    ~SDCardSPIAdapter();

    // Mounts the FAT filesystem into the ESP-IDF VFS
    bool mount();
    
    // Safely flushes and unmounts (critical for preventing FAT corruption)
    void unmount();

    bool is_ready() const { return is_mounted; }
};

}
