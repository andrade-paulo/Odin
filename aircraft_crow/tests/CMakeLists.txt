idf_component_register(
  SRCS 
    "producer_uart_hil_task.cpp"
    "consumer_uart_hil_task.cpp"
  INCLUDE_DIRS 
    "."
  REQUIRES 
    "domain"
    "esp_driver_uart"
)

target_include_directories(${COMPONENT_LIB} PRIVATE "../main")