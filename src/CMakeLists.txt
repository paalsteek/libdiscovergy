# -*- mode: cmake; -*-
set(get-sensor_src get_sensor.cpp)
set(post-sensor_src post_sensor.cpp)

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CURL_INCLUDE_DIR}
                    ${JSONCPP_INCLUDE_DIR}
)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

add_executable(get-sensor ${get-sensor_src})
add_executable(post-sensor ${post-sensor_src})

target_link_libraries(get-sensor mysmartgrid)
target_link_libraries(post-sensor mysmartgrid)

INSTALL(PROGRAMS 
  ${CMAKE_CURRENT_BINARY_DIR}/get-sensor
  ${CMAKE_CURRENT_BINARY_DIR}/post-sensor
  DESTINATION bin)
