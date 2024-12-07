#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chip8.h"
#include "instructions.h"

#include "raylib.h"

const float cycleThreshold = 1 / 700.0;
const float frameThreshold = 1 / 60.0;
const float timerThreshold = 1 / 60.0;

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

    // Setup raylib window
    const int windowWidth = 800;
    const int windowHeight = 450;
    InitWindow(windowWidth, windowHeight, "CHIP-8");

    // Init rand
    srand(time(NULL));

    // Chip-8
    Chip8 chip8 = initChip8();
    if (argc > 2) { chip8.isPaused = argv[2][0] == 'p'; }
    setup(&chip8, argv[1]);
    printf("Setup Complete!\n");

    // SetTargetFPS(60);
    double pCycleTime = GetTime();
    double pFrameTime = GetTime();
    double pTimerTime = GetTime();
    uint16_t pOpCode = 0;

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        updateKeyMap(chip8.kbd);

        bool isPaused = chip8.isPaused && !chip8.step;
        float delta = GetFrameTime();
        Op curOp = peekOp(chip8.cpu, chip8.ram);

        // Handle time and determine intent to avoid getting out of sync
        // (may not actally be and issue?)
        const double curTime = GetTime();
        const bool willCycleCpu = (curTime - pCycleTime) >= cycleThreshold && !isPaused;
        // const bool willCycleCpu = !isPaused;
        const bool willDrawFrame = (curTime - pFrameTime) >= frameThreshold && !isPaused;
        const bool willHandleTimers = (curTime - pTimerTime) >= timerThreshold && !isPaused;
        if (willCycleCpu) { pCycleTime = curTime; }
        if (willDrawFrame) { pFrameTime = curTime; }
        if (willHandleTimers) { pTimerTime = curTime; }

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
                printf("Prev OP: %x\n", pOpCode);
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

        draw(&chip8, curTime, pCycleTime, pFrameTime, delta, curOp.code, pOpCode);
        pOpCode = curOp.code;
    }

    CloseWindow();
    return 0;
}
