idf_component_register(
    SRCS "app_main.c"
    INCLUDE_DIRS ""
    REQUIRES lora_sx1276 mavlink_core udp_streamer freertos
)