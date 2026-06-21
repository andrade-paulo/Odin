cmake_minimum_required(VERSION 3.16)

list(APPEND EXTRA_COMPONENT_DIRS 
    "adapters/hardware_abstraction"
    "adapters/sensors"
    "adapters/comm"
    "adapters/sys"
    "domain"
    "tests"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(odin_telemetry)