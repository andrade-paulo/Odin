idf_component_register(
  SRCS "mavlink/mavlink_wrapper.cpp"
    "lora_task.cpp"
    # "pwm_task.cpp"
  INCLUDE_DIRS 
    "."
    "mavlink"
  REQUIRES 
    "domain" 
    "hardware_abstraction" 
    "freertos"
    "esp_timer"
)