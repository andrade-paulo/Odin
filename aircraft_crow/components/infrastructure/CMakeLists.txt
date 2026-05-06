# Create the directory during configuration so INCLUDE_DIRS doesn't crash
set(MAVLINK_OUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/mavlink_fw/generated")
file(MAKE_DIRECTORY ${MAVLINK_OUT_DIR})

# Register the ESP-IDF component
idf_component_register(
    SRCS "src/adapter_mpu6050.cpp"
         "src/adapter_sd_spi.cpp"
         "src/adapter_sx1276.cpp"
         "src/adapter_rc_receiver.cpp"
         "src/adapter_buzzer.cpp"
    INCLUDE_DIRS "include" "mavlink_fw/generated"
    REQUIRES core_domain driver fatfs sdmmc vfs esp_timer esp_driver_ledc
)

# Define the Python generation command
set(MAVLINK_XML "${CMAKE_CURRENT_SOURCE_DIR}/mavlink_fw/custom_telemetry.xml")

add_custom_command(
    OUTPUT ${MAVLINK_OUT_DIR}/custom_telemetry/mavlink.h
    COMMAND mavgen.py 
            --lang=C 
            --wire-protocol=2.0 
            --output=${MAVLINK_OUT_DIR} 
            ${MAVLINK_XML}
    DEPENDS ${MAVLINK_XML}
    COMMENT "Generating MAVLink v2.0 C headers from custom_telemetry.xml"
)

# Create a target for the generated files
add_custom_target(generate_mavlink_headers DEPENDS ${MAVLINK_OUT_DIR}/custom_telemetry/mavlink.h)

# Tell CMake: Do not compile this C++ component until the headers are generated!
add_dependencies(${COMPONENT_LIB} generate_mavlink_headers)
