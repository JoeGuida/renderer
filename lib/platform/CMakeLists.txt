cmake_minimum_required(VERSION 4.0)

set(WINDOWS_SOURCES
    src/windows/win32_window.cpp
)

if(WIN32)
    add_library(platform STATIC ${WINDOWS_SOURCES})
endif()

target_include_directories(platform PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(platform PUBLIC
    UNICODE
    _UNICODE
)

target_compile_options(platform PUBLIC
    "/utf-8"
)

