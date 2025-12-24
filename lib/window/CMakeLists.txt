cmake_minimum_required(VERSION 4.0)

add_library(window INTERFACE)

target_include_directories(window INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

