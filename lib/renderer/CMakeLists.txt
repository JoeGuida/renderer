cmake_minimum_required(VERSION 4.0)
project(renderer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(renderer STATIC
    src/command.cpp
    src/context.cpp
    src/debug_messenger.cpp
    src/device.cpp
    src/extension.cpp
    src/instance.cpp
    src/pipeline.cpp
    src/queue.cpp
    src/render_pass.cpp
    src/renderer.cpp
    src/shader.cpp
    src/surface.cpp
    src/swapchain.cpp
    src/sync.cpp
)

if(WIN32)
target_compile_definitions(renderer PRIVATE
    UNICODE
    _UNICODE
    WINDOWS
)

target_compile_options(renderer PRIVATE
    "/utf-8"
)
endif()

target_include_directories(renderer PUBLIC
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/../vulkan/include
)

find_package(spdlog CONFIG REQUIRED)

target_link_libraries(renderer PRIVATE
    platform
    window
    gdi32
    user32
    ${CMAKE_CURRENT_SOURCE_DIR}/../vulkan/lib/vulkan-1.lib
    spdlog::spdlog_header_only
)
