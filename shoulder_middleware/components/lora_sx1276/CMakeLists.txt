idf_component_register(
    SRCS "lora_sx1276.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_driver_spi esp_driver_gpio freertos esp_timer
)