cmake_minimum_required(VERSION 4.0)
project(triangle)

add_executable(triangle WIN32
    triangle.cpp
)

target_include_directories(triangle PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(triangle PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(triangle PRIVATE engine spdlog::spdlog_header_only opengl32.lib)

