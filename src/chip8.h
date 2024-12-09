#ifndef CHIP8_H
#define CHIP8_H

#include <stdbool.h>
#include <stdint.h>

#include "audio.h"
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
    Audio audio;

    // SuperChip
    bool hiRes;
    uint8_t flagRegisters[16];

    // Debug
    bool isPaused;
    uint8_t step;

    // Quirks
    bool resetVF;
    bool fromY;
    bool force0;
    bool carry;
    bool increment;
    bool clipping;
    bool dispWait;
}Chip8;

Chip8 initChip8();
void detatchChip8(Chip8* chip8);
void setup(Chip8* chip8, const char* romPath);

#endif
