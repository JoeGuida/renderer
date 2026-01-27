cmake_minimum_required(VERSION 4.0)
project(platform)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(WINDOWS_SOURCES
    src/windows/win32_window.cpp
)

add_library(platform STATIC ${WINDOWS_SOURCES})

target_include_directories(platform PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/windows
)

target_compile_definitions(platform PUBLIC
    UNICODE
    _UNICODE
    _WIN32
)

target_compile_options(platform PUBLIC
    "/utf-8"
)
