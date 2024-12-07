#ifndef CHIP8_H
#define CHIP8_H

#include <stdbool.h>
#include <stdint.h>

#include "cpu.h"
#include "ram.h"
#include "display.h"
#include "keyboard.h"

typedef struct chip_8 {
    Cpu* cpu;
    RAM* ram;
    Display* scr;
    Keyboard* kbd;

    bool isPaused;
    uint8_t step;
}Chip8;

#endif
