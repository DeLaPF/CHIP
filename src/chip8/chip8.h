#ifndef CHIP8_H
#define CHIP8_H

#include <stdbool.h>
#include <stdint.h>

#include "cpu.h"
#include "ram.h"
#include "keymap.h"
#include "vram.h"

// TODO: implement [XO-Chip](https://johnearnest.github.io/Octo/docs/XO-ChipSpecification.html)
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
    Op prevOp;

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
void Chip8LoadROM(Chip8* chip8, const char* romPath);
void Chip8SetVersion(Chip8* chip8, Chip8Version version);
void Chip8Step(Chip8* chip8);
void Chip8Reset(Chip8* chip8);
void Chip8Detatch(Chip8* chip8);

#endif
