set(MAVLINK_XML "${CMAKE_CURRENT_SOURCE_DIR}/custom_telemetry.xml")
set(MAVLINK_OUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generated")

add_custom_command(
    OUTPUT ${MAVLINK_OUT_DIR}/custom_dialect/mavlink.h
    COMMAND python3 -m pymavlink.tools.mavgen 
            --lang=C 
            --wire-protocol=2.0 
            --output=${MAVLINK_OUT_DIR} 
            ${MAVLINK_XML}
    DEPENDS ${MAVLINK_XML}
    COMMENT "Generating MAVLink v2.0 C headers from custom_telemetry.xml"
)

# Create a dummy target to force the generation
add_custom_target(generate_mavlink_headers DEPENDS ${MAVLINK_OUT_DIR}/custom_dialect/mavlink.h)
