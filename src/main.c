#include <stdint.h>
#include <stdio.h>

#include "raylib.h"

#include "constants.h"
#include "cpu.h"
#include "instructions.h"


const char* ROM_PATH = "./roms/IBM Logo.ch8";


const float cycleThreshold = 1 / 700.0;
const float frameThreshold = 1 / 60.0;
const float soundThreshold = 1 / 60.0;
const float delayThreshold = 1 / 60.0;

const int PADDING = 1;
const int DEBUG_INFO_WIDTH = 30;
const int DEBUG_INFO_HEIGHT = 54;
const int DEBUG_BUTTONS_WIDTH = 66;
const int DEBUG_BUTTONS_HEIGHT = 19;

struct chip_8 {
    struct cpu *cpu;
    uint8_t pixelBuff[2048];
};

void checkered(uint8_t* pixelBuff)
{
    for (int y = 0; y < BUFF_HEIGHT; y++) {
        for (int x = 0; x < BUFF_WIDTH; x++) {
            if (((x%2)+(y%2)+1)%2) {
                pixelBuff[y*BUFF_WIDTH+x] = 1;
            }
        }
    }
}

void draw(const uint8_t* pixelBuff)
{
    const double drawScale = 8.0;
    const double padding = PADDING*drawScale;
    BeginDrawing();
        ClearBackground(GRAY);

        // Draw Buff Frame
        const Vector2 buffFrameOrigin = { padding, padding };
        const double buffFrameWidth = (BUFF_WIDTH+2)*drawScale;
        const double buffFrameHeight = (BUFF_HEIGHT+2)*drawScale;
        DrawRectangle(
            buffFrameOrigin.x, buffFrameOrigin.y,
            buffFrameWidth, buffFrameHeight,
            RAYWHITE
        );

        // Draw Pixel Buffer to Buff Frame
        const Vector2 pixelBuffOrigin = {
            buffFrameOrigin.x+padding,
            buffFrameOrigin.y+padding
        };
        for (int y = 0; y < BUFF_HEIGHT; y++) {
            for (int x = 0; x < BUFF_WIDTH; x++) {
                if (pixelBuff[y*BUFF_WIDTH+x]) {
                    DrawRectangle(
                        pixelBuffOrigin.x+(x*drawScale),
                        pixelBuffOrigin.y+(y*drawScale),
                        drawScale,
                        drawScale,
                        BLACK
                    );
                }
            }
        }

        // Draw Debug Info Frame
        const Vector2 debugInfoFrameOrigin = {
            buffFrameOrigin.x+buffFrameWidth+padding,
            padding
        };
        const double debugInfoFrameHeight = DEBUG_INFO_HEIGHT*drawScale;
        const double debugInfoFrameWidth = DEBUG_INFO_WIDTH*drawScale;
        DrawRectangle(
            debugInfoFrameOrigin.x, debugInfoFrameOrigin.y,
            debugInfoFrameWidth, debugInfoFrameHeight,
            RAYWHITE
        );

        // TODO: Draw Debug info (fps/mspt, registers, stack(ptr), etc.)

        // Draw Debug Buttons Frame
        const Vector2 debugButtonsFrameOrigin = {
            padding,
            buffFrameOrigin.y+buffFrameHeight+padding
        };
        const double debugButtonsFrameHeight = DEBUG_BUTTONS_HEIGHT*drawScale;
        const double debugButtonsFrameWidth = DEBUG_BUTTONS_WIDTH*drawScale;
        DrawRectangle(
            debugButtonsFrameOrigin.x, debugButtonsFrameOrigin.y,
            debugButtonsFrameWidth, debugButtonsFrameHeight,
            RAYWHITE
        );

        // TODO: Draw Debug Buttons (to enable pause/play, step, etc.)

    EndDrawing();
}

void handleSound()
{
    // TODO: impl
}

int main(void)
{
    const int windowWidth = 800;
    const int windowHeight = 450;
    InitWindow(windowWidth, windowHeight, "CHIP-8");

    // Chip-8
    struct cpu cpu = {
        {0},  // registers[16],
        0,    // idx,
        {0},  // stack[64],
        0,    // sp,
        0,    // pc,
        {0},  // heap[4096],
        0,    // delayT,
        0,    // soundT
    };
    struct chip_8 chip8 = {
        &cpu,
        { 0 },
    };
    checkered(chip8.pixelBuff);

    // SetTargetFPS(60);
    double prevCycleTime = GetTime();
    double prevFrameTime = GetTime();
    double prevSoundTime = GetTime();
    double prevDelayTime = GetTime();

    printf("Loading ROM...\n");

    // Load bytes from rom into heap
    uint8_t* buffer = &chip8.cpu->heap[PROG_START];
    FILE* fileptr = fopen(ROM_PATH, "rb");
    fseek(fileptr, 0, SEEK_END);
    long filelen = ftell(fileptr);
    rewind(fileptr);
    fread(buffer, filelen, 1, fileptr);
    fclose(fileptr);

    // Set pc
    chip8.cpu->pc = PROG_START;

    printf("Setup Complete!\n");

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        // Handle time and determine intent to avoid getting out of sync
        // (may not actally be and issue?)
        const double curTime = GetTime();
        const bool willCycleCpu = (curTime - prevCycleTime) >= cycleThreshold;
        const bool willDrawFrame = (curTime - prevFrameTime) >= frameThreshold;
        const bool willHandleSound = (curTime - prevSoundTime) >= soundThreshold;
        if (willCycleCpu) { prevCycleTime = curTime; }
        if (willDrawFrame) { prevFrameTime = curTime; }
        if (willHandleSound) { prevSoundTime = curTime; }

        // Handle intent
        if (willCycleCpu) {
            // Fetch
            uint16_t opCode = chip8.cpu->heap[chip8.cpu->pc] << 8;
            opCode |= chip8.cpu->heap[chip8.cpu->pc+1];
            chip8.cpu->pc += 2;

            // Decode and Execute
            uint8_t opNib = opCode&0xF000 >> 12;
            uint8_t x = opCode&0x0F00 >> 8;
            uint8_t y = opCode&0x00F0 >> 4;
            uint8_t n = opCode&0x000F;
            uint8_t nn = opCode&0x00FF;
            uint16_t nnn = opCode&0x0FFF;
            switch (opNib) {
                case 0x0:
                    switch (nn) {
                        case 0xE0:
                            clearScreen(chip8.pixelBuff);
                            break;
                        case 0xEE:
                            break;
                        default:
                            // TODO: error
                            break;
                    }
                    // code block
                    break;
                case 0x1:
                    jump(chip8.cpu, nnn);
                    break;
                case 0x2:
                    // code block
                    break;
                case 0x3:
                    // code block
                    break;
                case 0x4:
                    // code block
                    break;
                case 0x5:
                    // code block
                    break;
                case 0x6:
                    setVX(chip8.cpu, x, nn);
                    break;
                case 0x7:
                    addToVX(chip8.cpu, x, nn);
                    break;
                case 0x8:
                    // code block
                    break;
                case 0x9:
                    // code block
                    break;
                case 0xA:
                    setIdx(chip8.cpu, nnn);
                    break;
                case 0xB:
                    // code block
                    break;
                case 0xC:
                    // code block
                    break;
                case 0xD:
                    updateBuffer(chip8.pixelBuff, chip8.cpu, x, y, n);
                    break;
                case 0xE:
                    // code block
                    break;
                case 0xF:
                    // code block
                    break;
                default:
                    // TODO: error
                    break;
            };
        }
        if (willDrawFrame) {
            // TODO: update pixelBuff
        }
        if (willHandleSound) { handleSound(); }

        draw(chip8.pixelBuff);
    }

    CloseWindow();

    return 0;
}
