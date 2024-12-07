#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chip8.h"
#include "constants.h"
#include "instructions.h"

#include "raylib.h"

const float cycleThreshold = 1 / 700.0;
const float frameThreshold = 1 / 60.0;
const float timerThreshold = 1 / 60.0;

void setup(Chip8* chip8, const char* romPath) {
    printf("Loading Font...\n");
    loadFont(chip8->ram);

    printf("Loading ROM %s...\n", romPath);
    loadROM(chip8->ram, romPath);

    chip8->cpu->pc = PROG_START;
}

void handleSound()
{
    // TODO: impl
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Error: specify ROM PATH");
        return 0;
    }

    const int windowWidth = 800;
    const int windowHeight = 450;
    InitWindow(windowWidth, windowHeight, "CHIP-8");

    // Init rand
    srand(time(NULL));

    // Chip-8
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
        .cpu=&cpu,
        .ram=&ram,
        .scr=&scr,
        .kbd=&kbd,
        .isPaused=false,
        .step=0,
        .resetVF=true,
        .fromY=false,
        .force0=false,
        .carry=true,
        .increment=true,
    };
    if (argc > 2) { chip8.isPaused = argv[2][0] == 'p'; }

    setup(&chip8, argv[1]);
    printf("Setup Complete!\n");

    // SetTargetFPS(60);
    double prevCycleTime = GetTime();
    double prevFrameTime = GetTime();
    double prevTimerTime = GetTime();

    uint16_t prevOpCode = 0;
    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        updateKeyMap(chip8.kbd);
        bool isPaused = chip8.isPaused && !chip8.step;
        // Handle time and determine intent to avoid getting out of sync
        // (may not actally be and issue?)
        const double curTime = GetTime();
        const bool willCycleCpu = (curTime - prevCycleTime) >= cycleThreshold && !isPaused;
        // const bool willCycleCpu = !isPaused;
        const bool willDrawFrame = (curTime - prevFrameTime) >= frameThreshold && !isPaused;
        const bool willHandleTimers = (curTime - prevTimerTime) >= timerThreshold && !isPaused;
        if (willCycleCpu) { prevCycleTime = curTime; }
        if (willDrawFrame) { prevFrameTime = curTime; }
        if (willHandleTimers) { prevTimerTime = curTime; }

        // For Debug
        Op curOp = peekOp(chip8.cpu, chip8.ram);

        // Handle intent
        if (willCycleCpu) {
            if (chip8.step) { chip8.step--; }

            // Fetch
            Op op = fetchOp(chip8.cpu, chip8.ram);

            // Decode
            Instruction instruction = decode(op.nib, op.n, op.nn);

            // Execute
            if (!instruction) {
                printf("Error Unknown Instruction: 0x%x\n", op.code);
                printf("Prev OP: %x\n", prevOpCode);
                chip8.isPaused = true;
            } else {
                instruction(&chip8, op.x, op.y, op.n, op.nn, op.nnn);
            }
        }
        if (willDrawFrame) {
            // TODO: update pixelBuff
        }
        if (willHandleTimers) {
            handleSound();
            if (chip8.ram->delayTimer) { chip8.ram->delayTimer--; }
            if (chip8.ram->soundTimer) { chip8.ram->soundTimer--; }
        }

        draw(
            &chip8,
            curTime,
            prevCycleTime,
            prevFrameTime,
            GetFrameTime(),
            curOp.code
        );
        prevOpCode = curOp.code;
    }

    CloseWindow();

    return 0;
}
