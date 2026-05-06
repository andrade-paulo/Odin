idf_component_register(
    SRCS "main.cpp"
         "rtos_tasks/task_acquisition.cpp"
         "rtos_tasks/task_sd_logger.cpp"
         "rtos_tasks/task_pwm_logger.cpp"
         "rtos_tasks/task_lora_tx.cpp"
         "rtos_tasks/task_buzzer.cpp"
    INCLUDE_DIRS "."
    REQUIRES core_domain infrastructure nvs_flash esp_timer
)
