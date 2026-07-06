cmake_minimum_required(VERSION 4.0)
project(renderer-lib)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(renderer-lib STATIC
    src/gl_loader.cpp
    src/renderer.cpp
)

target_include_directories(renderer-lib PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(glm CONFIG REQUIRED)
target_link_libraries(renderer-lib PRIVATE glm::glm window)

