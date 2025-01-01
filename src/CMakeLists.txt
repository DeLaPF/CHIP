set(chip8_sources
    chip8/chip8.c
    chip8/constants.c
    chip8/cpu.c
    chip8/instructions.c
    chip8/keymap.c
    chip8/ram.c
    chip8/vram.c
)
add_library(${PROJECT_NAME} STATIC ${chip8_sources})
target_include_directories(${PROJECT_NAME} PUBLIC .)
