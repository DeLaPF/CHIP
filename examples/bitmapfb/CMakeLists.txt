set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bmpchip/bin)
set (CMAKE_CXX_STANDARD 20)

set(bmpchip_sources
    app.cpp

    keyboard.cpp
)
add_executable(bmpChip ${bmpchip_sources})
target_include_directories(bmpChip PRIVATE .)

target_link_libraries(bmpChip PRIVATE ${PROJECT_NAME})
target_link_libraries(bmpChip PRIVATE bmp_fb::Entrypoint)
