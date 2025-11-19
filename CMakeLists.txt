cmake_minimum_required(VERSION 4.0)
project(renderer)
file(GLOB_RECURSE sources src/*.cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(renderer ${sources})

target_compile_definitions(renderer PRIVATE UNICODE _UNICODE)
target_compile_options(renderer PRIVATE "/utf-8")

target_include_directories(renderer PRIVATE include deps deps/opengl)

target_link_libraries(renderer PRIVATE gdi32.lib user32.lib opengl32.lib ${CMAKE_SOURCE_DIR}/deps/spdlog.lib)

set_target_properties(renderer PROPERTIES WIN32_EXECUTABLE True)

