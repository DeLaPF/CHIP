#include "emu.h"

#include <stdio.h>

#include "raylib.h"

#include "chip8/chip8.h"
#include "chip8/instructions.h"
#include "raylib/audio.h"

const double cycleThreshold = 1 / 700.0;
// const double frameThreshold = 1 / 60.0;
// TODO: Possibly use accumulator instead of hard cuttoff
const double frameThreshold = 1 / 120.0;
const double timerThreshold = 1 / 60.0;

void run(char* romPath, bool startPaused)
{
    // Setup raylib audio and window
    const int windowWidth = 800;
    const int windowHeight = 450;
    InitWindow(windowWidth, windowHeight, "CHIP-8");
    InitAudioDevice();
    Audio audio = initAudio();

    // Chip-8
    Chip8 chip8 = initChip8();
    chip8.isPaused = startPaused;
    setup(&chip8, romPath);
    printf("Setup Complete!\n");

    double pCycleTime = -1.0;
    double pFrameTime = -1.0;
    double pLoopTime = -1.0;
    double pTimerTime = -1.0;
    uint16_t pOpCode = 0;

    // Main game loop
    while (!WindowShouldClose()) {
        updateKeyMap(&chip8.kbd);

        double curTime = GetTime();
        float delta = curTime - pLoopTime;
        Op curOp = peekOp(&chip8.cpu, &chip8.ram);

        if (!chip8.isPaused || chip8.step) {
            bool wait = chip8.dispWait && (curTime - pFrameTime) < frameThreshold;
            if ((curTime - pCycleTime) >= cycleThreshold && !wait) {
                // Fetch
                Op op = fetchOp(&chip8.cpu, &chip8.ram);
                // Decode
                Instruction instruction = decode(&op);
                // Execute
                if (!instruction) {
                    printf("Error Unknown Instruction: 0x%04x\n", op.code);
                    printf("Prev OP: 0x%04x\n", pOpCode);
                    chip8.isPaused = true;
                } else {
                    instruction(&chip8, &op);
                }

                if (op.nib == 0xD && !chip8.hiRes) { pFrameTime = curTime; }
                pCycleTime = curTime;
            }
            if ((curTime - pTimerTime) >= timerThreshold) {
                if (chip8.ram.soundTimer) {
                    playAudio(&audio);
                } else {
                    pauseAudio(&audio);
                }

                if (chip8.ram.delayTimer) { chip8.ram.delayTimer--; }
                if (chip8.ram.soundTimer) { chip8.ram.soundTimer--; }
                pTimerTime = curTime;
            }

            if (chip8.step) { chip8.step--; }
            pOpCode = curOp.code;
        }

        updateAudio(&audio);
        draw(&chip8, delta);
        // draw_debug(&chip8, curTime, pCycleTime, pFrameTime, delta, curOp.code, pOpCode);
        pLoopTime = curTime;
    }

    detachAudio(&audio);
    detatchChip8(&chip8);
    CloseAudioDevice();
    CloseWindow();
}
