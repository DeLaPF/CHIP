#include "chip8.h"

#include <stdio.h>

#include "chip8/constants.h"

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
        .width=CHIP8_BUFF_WIDTH,
        .height=CHIP8_BUFF_HEIGHT,
    };
    Keyboard kbd = {
        .keyMap=0,
    };
    // TODO: add method for setting quirks at runtime (config file or args?)
    Chip8 chip8 = {
        .cpu=cpu,
        .ram=ram,
        .scr=scr,
        .kbd=kbd,
        .audio=initAudio(),
        .hiRes=false,
        .flagRegisters={0},
        .isPaused=false,
        .step=0,
        .resetVF=false,
        .fromY=false,
        .force0=false,
        .carry=false,
        .increment=false,
        .clipping=true,
        .dispWait=false,
    };
    // CHIP8
    // .resetVF=true,
    // .fromY=true,
    // .force0=true,
    // .carry=false,
    // .increment=true,
    // .clipping=true,
    // .dispWait=true,

    // SUPERCHIP (legacy)
    // .resetVF=false,
    // .fromY=false,
    // .force0=false,
    // .carry=false,
    // .increment=false,
    // .clipping=true,
    // .dispWait=true,

    // SUPERCHIP (modern)
    // .resetVF=false,
    // .fromY=false,
    // .force0=false,
    // .carry=false,
    // .increment=false,
    // .clipping=true,
    // .dispWait=false,

    return chip8;
}

void detatchChip8(Chip8* chip8)
{
    detachAudio(&chip8->audio);
}

void setup(Chip8* chip8, const char* romPath)
{
    printf(
        "Loading Lo Font: 0x%x to 0x%x...\n",
        LO_FONT_START,
        LO_FONT_START+LO_FONT_BYTES
    );
    loadLoFont(&chip8->ram);
    printf(
        "Loading Hi Font: 0x%x to 0x%x...\n",
        HI_FONT_START,
        HI_FONT_START+HI_FONT_BYTES
    );
    printf("Loading Hi Font...\n");
    loadHiFont(&chip8->ram);

    printf("Loading ROM %s...\n", romPath);
    loadROM(&chip8->ram, romPath);

    chip8->cpu.pc = PROG_START;
}
