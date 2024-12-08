#include "chip8.h"

#include <stdio.h>

#include "constants.h"


Chip8 initChip8()
{
    Cpu cpu = {
        .registers={0},
        .idx=0,
        .sp=0,
        .pc=0,
    };
    RAM ram = {
        .delayTimer=0,
        .soundTimer=0,
        .stack={0},
        .heap={0},
    };
    Display scr = {
        .pixelBuff={0},
    };
    Keyboard kbd = {
        .keyMap=0,
    };
    Chip8 chip8 = {
        .cpu=cpu,
        .ram=ram,
        .scr=scr,
        .kbd=kbd,
        .isPaused=false,
        .step=0,
        .resetVF=true,
        .fromY=false,
        .force0=false,
        .carry=true,
        .increment=true,
    };

    return chip8;
}

void setup(Chip8* chip8, const char* romPath)
{
    printf("Loading Font...\n");
    loadFont(&chip8->ram);

    printf("Loading ROM %s...\n", romPath);
    loadROM(&chip8->ram, romPath);

    chip8->cpu.pc = PROG_START;
}
