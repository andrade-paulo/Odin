idf_component_register(
    SRCS "imu_task.cpp" "barometer_task.cpp" "gps_task.cpp"
    INCLUDE_DIRS "."
    REQUIRES domain hardware_abstraction sys esp_driver_uart freertos esp_timer
)