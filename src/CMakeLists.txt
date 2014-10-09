# -*- mode: cmake; -*-
set(get-sensor_src get_sensor.cpp)

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CURL_INCLUDE_DIR}
                    ${JSONCPP_INCLUDE_DIR}
)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

add_executable(get-sensor ${get-sensor_src})

target_link_libraries(get-sensor discovergy)

INSTALL(PROGRAMS 
  ${CMAKE_CURRENT_BINARY_DIR}/get-sensor
  DESTINATION bin)
