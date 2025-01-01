cmake_minimum_required(VERSION 3.13)
set(PROJECT_NAME CHIP-8)
project(${PROJECT_NAME})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES
    src/main.c

    src/chip8/chip8.c
    src/chip8/constants.c
    src/chip8/cpu.c
    src/chip8/instructions.c
    src/chip8/keymap.c
    src/chip8/ram.c
    src/chip8/vram.c

    src/raylib/audio.c
    src/raylib/display.c
    src/raylib/emu.c
    src/raylib/keyboard.c
)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)

add_subdirectory(deps/raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
