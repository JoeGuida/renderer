cmake_minimum_required(VERSION 4.0)
project(window)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(window STATIC src/win32_window.cpp)

target_compile_definitions(window PUBLIC
    UNICODE
    _UNICODE
)

target_include_directories(window PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(window PUBLIC input)

