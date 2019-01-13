cmake_minimum_required(VERSION 3.0)
project(mqtt_paho_client)
add_definitions("-std=c++14")

#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup()


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(Paho)
include_directories(${CMAKE_SOURCE_DIR}/include
                    ${PAHO_INCLUDE_DIRS})

add_library(mqtt_pub
    ${CMAKE_SOURCE_DIR}/src/mqtt_pub.cpp
  )

target_link_libraries(mqtt_pub
    ${PAHO_LIBRARIES}
  )

add_executable(mqtt_pub_client
    ${CMAKE_SOURCE_DIR}/src/mqtt_pub_client.cpp
  )

#add_executable(mqtt_sub_client
#  ${CMAKE_SOURCE_DIR}/src/mqtt_sub.cpp
#  ${CMAKE_SOURCE_DIR}/src/mqtt_sub_client.cpp
#  )

target_link_libraries(mqtt_pub_client
  mqtt_pub
  )

#target_link_libraries(mqtt_sub_client ${CONAN_LIBS})
