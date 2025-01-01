cmake_minimum_required(VERSION 3.13)
set(PROJECT_NAME CHIP-8)
project(${PROJECT_NAME})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE DEBUG)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    # TODO: enable options for building different examples
    add_subdirectory(deps/raylib)
    add_subdirectory(examples/raylib)
endif()

add_subdirectory(src chip8)
