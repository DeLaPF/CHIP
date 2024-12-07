cmake_minimum_required(VERSION 3.13)
set(PROJECT_NAME CHIP-8)
project(${PROJECT_NAME})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES
    src/main.c
	src/constants.c
    src/cpu.c
	src/display.c
	src/instructions.c
    src/keyboard.c
    src/ram.c
)
set(HEADER_FILES
    src/constants.h
	src/cpu.h
	src/display.h
	src/instructions.h
    src/keyboard.h
	src/ram.h
)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE src)

set(CMAKE_BUILD_TYPE DEBUG)
add_subdirectory(third_party/raylib)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib)
