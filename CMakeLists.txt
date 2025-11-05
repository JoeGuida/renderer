cmake_minimum_required(VERSION 4.0)
project(gl_renderer)
file(GLOB_RECURSE sources src/*.cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(gl_renderer ${sources})

target_compile_definitions(gl_renderer PRIVATE UNICODE _UNICODE)
target_compile_options(gl_renderer PRIVATE "/utf-8")

target_include_directories(gl_renderer PRIVATE include deps deps/opengl)

target_link_libraries(gl_renderer PRIVATE gdi32.lib user32.lib opengl32.lib ${CMAKE_SOURCE_DIR}/deps/spdlogd.lib)

set_target_properties(gl_renderer PROPERTIES WIN32_EXECUTABLE True)

