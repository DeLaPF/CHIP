#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chip8.h"
#include "instructions.h"

#include "raylib.h"

const double cycleThreshold = 1 / 700.0;
// const double frameThreshold = 1 / 60.0;
// TODO: Possibly use accumulator instead of hard cuttoff
const double frameThreshold = 1 / 120.0;
const double timerThreshold = 1 / 60.0;

void handleSound(Chip8* chip8)
{
    if (chip8->ram.soundTimer) {
        playAudio(&chip8->audio);
    } else {
        pauseAudio(&chip8->audio);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Error: specify ROM PATH");
        return 0;
    }

    // Setup raylib audio and window
    const int windowWidth = 800;
    const int windowHeight = 450;
    InitWindow(windowWidth, windowHeight, "CHIP-8");
    InitAudioDevice();

    // Init rand
    srand(time(NULL));

    // Chip-8
    Chip8 chip8 = initChip8();
    if (argc > 2) { chip8.isPaused = argv[2][0] == 'p'; }
    setup(&chip8, argv[1]);
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
                handleSound(&chip8);

                if (chip8.ram.delayTimer) { chip8.ram.delayTimer--; }
                if (chip8.ram.soundTimer) { chip8.ram.soundTimer--; }
                pTimerTime = curTime;
            }

            if (chip8.step) { chip8.step--; }
            pOpCode = curOp.code;
        }

        updateAudio(&chip8.audio);
        draw(&chip8, delta);
        // draw_debug(&chip8, curTime, pCycleTime, pFrameTime, delta, curOp.code, pOpCode);
        pLoopTime = curTime;
    }

    detatchChip8(&chip8);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
