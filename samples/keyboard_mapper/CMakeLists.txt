cmake_minimum_required(VERSION 4.0)
project(keyboard_mapper)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(keyboard_mapper main.cpp)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(keyboard_mapper PRIVATE input spdlog::spdlog_header_only)
