cmake_minimum_required(VERSION 3.10)

project(DetectLights)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCES lightcode.cpp)

add_executable(detect_lights ${SOURCES})

target_link_libraries(detect_lights ${OpenCV_LIBS})