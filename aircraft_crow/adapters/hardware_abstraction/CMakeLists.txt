idf_component_register(
    SRCS "sx1276_driver.cpp" "i2c_bus_manager.cpp"
    INCLUDE_DIRS "."
    REQUIRES 
        "driver" 
        "esp_driver_spi" 
        "esp_driver_gpio"
        "freertos" 
        "esp_timer"
)