#ifndef CHIP8_H
#define CHIP8_H

#include <stdbool.h>
#include <stdint.h>

#include "cpu.h"
#include "ram.h"
#include "display.h"
#include "keyboard.h"

typedef struct chip8 {
    // Hardware
    Cpu cpu;
    RAM ram;
    Display scr;
    Keyboard kbd;

    // Debug
    bool isPaused;
    uint8_t step;

    // Quirks
    bool resetVF;
    bool fromY;
    bool force0;
    bool carry;
    bool increment;
    bool dispWait;
}Chip8;

Chip8 initChip8();
void setup(Chip8* chip8, const char* romPath);

#endif
