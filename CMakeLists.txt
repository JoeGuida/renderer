cmake_minimum_required(VERSION 4.0)

project(renderer)

file(GLOB_RECURSE sources src/*.cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(lib/platform)
add_subdirectory(lib/window)

add_executable(renderer
    ${sources}
)

target_compile_definitions(renderer PRIVATE
    UNICODE
    _UNICODE
)

target_compile_options(renderer PRIVATE
    "/utf-8"
)

target_include_directories(renderer PRIVATE
    include
    include/samples
    ${CMAKE_SOURCE_DIR}/lib/vulkan/include
)

find_package(spdlog CONFIG REQUIRED)

target_link_libraries(renderer PRIVATE
    platform
    window
    gdi32
    user32
    opengl32
    ${CMAKE_SOURCE_DIR}/lib/vulkan/lib/vulkan-1.lib
    spdlog::spdlog_header_only
)

set_target_properties(renderer PROPERTIES WIN32_EXECUTABLE True)

