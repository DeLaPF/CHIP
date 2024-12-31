#include "chip8.h"

#include <stdio.h>

#include "chip8/constants.h"
#include "chip8/instructions.h"

static void loadFonts(Chip8* chip8)
{
    // TODO: remove debug prints
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
}

Chip8 makeChip8()
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
    VRAM vram = {
        .pixelBuff={0},
        .width=CHIP8_BUFF_WIDTH,
        .height=CHIP8_BUFF_HEIGHT,
    };
    Keymap keymap = 0;
    Chip8 chip8 = {
        .cpu=cpu,
        .ram=ram,
        .vram=vram,
        .keymap=keymap,
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
    loadFonts(&chip8);

    return chip8;
}

// Note: should call reset before using to avoid undefined state
// (ignore if just created with makeChip8)
void Chip8LoadROM(Chip8* chip8, const char* romPath)
{
    printf("Loading ROM %s...\n", romPath);
    loadROM(&chip8->ram, romPath);

    chip8->cpu.pc = PROG_START;
}

// Note: should call reset before using to avoid undefined state
void Chip8SetVersion(Chip8* chip8, Chip8Version version)
{
    switch (version) {
    case CHIP8:
        chip8->resetVF=true;
        chip8->fromY=true;
        chip8->force0=true;
        chip8->carry=false;
        chip8->increment=true;
        chip8->clipping=true;
        chip8->dispWait=true;
        break;
    case SCHIP_LEGACY:
        chip8->resetVF=false;
        chip8->fromY=false;
        chip8->force0=false;
        chip8->carry=false;
        chip8->increment=false;
        chip8->clipping=true;
        chip8->dispWait=true;
        break;
    case SCHIP_MODERN:
        chip8->resetVF=false;
        chip8->fromY=false;
        chip8->force0=false;
        chip8->carry=false;
        chip8->increment=false;
        chip8->clipping=true;
        chip8->dispWait=false;
        break;
    default:
        printf("ERROR [CHIP8 INIT]: Unknown version '%d'", version);
    }
}

void Chip8Step(Chip8* chip8)
{
    // Fetch
    Op op = fetchOp(&chip8->cpu, &chip8->ram);
    // Decode
    Instruction instruction = decode(&op);
    // Execute
    if (!instruction) {
        printf("Error Unknown Instruction: 0x%04x\n", op.code);
        printf("Prev OP: 0x%04x\n", chip8->prevOp.code);
        chip8->isPaused = true;
    } else {
        instruction(chip8, &op);
    }

    chip8->prevOp = op;
}

void Chip8Reset(Chip8* chip8)
{
    // TODO: switch to managed memory model and memset all data to zero
    loadFonts(chip8);
}

void Chip8Detatch(Chip8* chip8)
{
    // TODO: handle freeing memory (once switch over heap based mem model)
}
