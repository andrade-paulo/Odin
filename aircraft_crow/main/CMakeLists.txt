idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "."
    REQUIRES domain comm sensors hardware_abstraction sys tests freertos 
)