set(chip8_public_headers
    chip8/chip8.h
    # TODO: separate public and non

    # chip8/constants.h
    # chip8/cpu.h
    # chip8/instructions.h
    # chip8/keymap.h
    # chip8/ram.h
    # chip8/vram.h
)
set(chip8_library_sources
    chip8/chip8.c
    chip8/constants.c
    chip8/cpu.c
    chip8/instructions.c
    chip8/keymap.c
    chip8/ram.c
    chip8/vram.c
)
add_library(${PROJECT_NAME} STATIC ${chip8_library_sources} ${chip8_public_headers})
# target_include_directories(${PROJECT_NAME} PRIVATE .)
target_include_directories(${PROJECT_NAME} PUBLIC .)

if (CHIP8_IS_MAIN)
    add_subdirectory(raylib)
endif()
