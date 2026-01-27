cmake_minimum_required(VERSION 4.0)
project(window)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(window INTERFACE)

target_include_directories(window INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

