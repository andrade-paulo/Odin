cmake_minimum_required(VERSION 3.22)

set(EXTRA_COMPONENT_DIRS
	${CMAKE_SOURCE_DIR}/domain
	${CMAKE_SOURCE_DIR}/adapters/comm
	${CMAKE_SOURCE_DIR}/adapters/sensors
	${CMAKE_SOURCE_DIR}/main
	${CMAKE_SOURCE_DIR}/tests
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(aircraft_crow)
