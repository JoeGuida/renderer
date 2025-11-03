cmake_minimum_required(VERSION 4.0)
project(3DR)
file(GLOB_RECURSE sources src/*.cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(3DR ${sources})

target_compile_definitions(3DR PRIVATE UNICODE _UNICODE)
target_compile_options(3DR PRIVATE "/utf-8")

target_include_directories(3DR PRIVATE include deps deps/opengl)

target_link_libraries(3DR PRIVATE gdi32.lib user32.lib opengl32.lib ${CMAKE_SOURCE_DIR}/deps/spdlogd.lib)

set_target_properties(3DR PROPERTIES WIN32_EXECUTABLE True)

