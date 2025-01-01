#ifndef CHIP8_H
#define CHIP8_H

#include <stdbool.h>
#include <stdint.h>

#include "cpu.h"
#include "ram.h"
#include "keymap.h"
#include "vram.h"

#define CHIP8_NUM_FLAG_REGISTERS 16

// TODO: implement [XO-Chip](https://johnearnest.github.io/Octo/docs/XO-ChipSpecification.html)
typedef enum version {
    CHIP8,
    SCHIP_LEGACY,
    SCHIP_MODERN
}Chip8Version;

typedef struct display {
    uint16_t width;
    uint16_t height;
}Chip8Display;

typedef struct chip8 {
    // Hardware
    CPU cpu;
    RAM ram;
    VRAM vram;
    Keymap keymap;
    Chip8Display display;

    // SuperChip
    bool hiRes;
    uint8_t flagRegisters[CHIP8_NUM_FLAG_REGISTERS];

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
void Chip8Init(Chip8* chip8);
void Chip8Destroy(Chip8* chip8);

void Chip8LoadROM(Chip8* chip8, const char* romPath);
void Chip8SetVersion(Chip8* chip8, Chip8Version version);
void Chip8Step(Chip8* chip8);

void Chip8LoadFlagRegisters(Chip8* chip8, uint8_t* regVals);

#endif
