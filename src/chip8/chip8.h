#ifndef CHIP8_H
#define CHIP8_H

#include <stdbool.h>
#include <stdint.h>

#include "cpu.h"
#include "ram.h"
#include "keymap.h"
#include "vram.h"

typedef enum version {
    CHIP8,
    SCHIP_LEGACY,
    SCHIP_MODERN
}Chip8Version;

typedef struct chip8 {
    // Hardware
    Cpu cpu;
    RAM ram;
    VRAM vram;
    Keymap keymap;

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

Chip8 makeChip8();
void loadROMChip8(Chip8* chip8, const char* romPath);
void setVersionChip8(Chip8* chip8, Chip8Version version);
void resetChip8(Chip8* chip8);
void detatchChip8(Chip8* chip8);

#endif
