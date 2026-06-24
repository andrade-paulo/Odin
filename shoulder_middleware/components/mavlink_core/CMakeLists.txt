idf_component_register(
    SRCS "mavlink_core.c"
    INCLUDE_DIRS "include" "include/generated"
    REQUIRES freertos 
)