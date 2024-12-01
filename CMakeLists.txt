cmake_minimum_required(VERSION 3.13)
set(PROJECT_NAME CHIP-8)
project(${PROJECT_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES src/main.c)
add_executable(${PROJECT_NAME} ${SOURCES})

set(CMAKE_BUILD_TYPE DEBUG)
add_subdirectory(third_party/raylib)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib)
