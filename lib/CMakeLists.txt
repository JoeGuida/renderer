cmake_minimum_required(VERSION 4.0)
project(engine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(platform)
add_subdirectory(window)
add_subdirectory(renderer)

add_library(engine INTERFACE)

target_link_libraries(engine INTERFACE platform window renderer)
