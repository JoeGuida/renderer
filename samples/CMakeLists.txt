cmake_minimum_required(VERSION 4.0)

add_executable(triangle WIN32
    triangle/triangle.cpp
)

find_package(spdlog CONFIG REQUIRED)

target_link_libraries(triangle PRIVATE
    engine
    spdlog::spdlog_header_only
)
