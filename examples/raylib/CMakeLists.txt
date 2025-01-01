set(raychip_sources
    main.c

    audio.c
    display.c
    emu.c
    keyboard.c
)
add_executable(rayChip ${raychip_sources})
target_include_directories(rayChip PRIVATE .)

target_link_libraries(rayChip PRIVATE ${PROJECT_NAME})
target_link_libraries(rayChip PRIVATE raylib)
