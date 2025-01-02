#include "chip8.h"

#include <stdio.h>

#include "constants.h"
#include "instructions.h"

Chip8 makeChip8()
{
    Chip8 chip8 = {
        .cpu=makeCPU(),
        .ram=makeRAM(4096),
        .vram=makeVRAM(SUPER_CHIP_BUFF_WIDTH*SUPER_CHIP_BUFF_HEIGHT),
        .display={ .width=CHIP8_BUFF_WIDTH, .height=CHIP8_BUFF_HEIGHT, .didUpdate=false },
        .keymap=0,
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

    return chip8;
}

void Chip8Init(Chip8* chip8)
{
    chip8->keymap = 0;
    chip8->hiRes = false;

    CPUInit(&chip8->cpu);
    RAMInit(&chip8->ram);
    VRAMInit(&chip8->vram);
}

void Chip8Destroy(Chip8* chip8)
{
    RAMDestroy(&chip8->ram);
    VRAMDestroy(&chip8->vram);
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
    chip8->display.didUpdate = false;

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

void Chip8LoadFlagRegisters(Chip8* chip8, uint8_t* regVals)
{
    for (int i = 0; i < CHIP8_NUM_FLAG_REGISTERS; i++) {
        chip8->flagRegisters[i] = regVals[i];
    }
}
