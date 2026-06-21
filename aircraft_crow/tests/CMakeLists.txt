idf_component_register(
    SRCS "consumer_uart_hil_task.cpp"
    INCLUDE_DIRS "."
    REQUIRES domain esp_driver_uart freertos
)