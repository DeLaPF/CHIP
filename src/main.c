#include <stdint.h>

#include "raylib.h"


const float frameThreshold = 1 / 60.0;
const float soundThreshold = 1 / 60.0;
const float delayThreshold = 1 / 60.0;

const int PADDING = 1;
const int DEBUG_INFO_WIDTH = 30;
const int DEBUG_INFO_HEIGHT = 54;
const int DEBUG_BUTTONS_WIDTH = 66;
const int DEBUG_BUTTONS_HEIGHT = 19;

const int BUFF_WIDTH = 64;
const int BUFF_HEIGHT = 32;

struct chip_8 {
    uint8_t registers[16];
    uint8_t pixelBuff[2048];
};

struct cpu {
    uint8_t registers[16];
    uint16_t idxReg;
    uint8_t stack[64];
    uint8_t stackPtr;
    uint16_t programCtr;
    uint8_t heap[4096];
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

void drawScreen(const uint8_t* pixelBuff)
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

    EndDrawing();
}

void draw()
{
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
    struct chip_8 chip8 = {
        { 0 },
        { 0 },
    };
    checkered(chip8.pixelBuff);

    // SetTargetFPS(60);
    double prevFrameTime = GetTime();
    double prevSoundTime = GetTime();
    double prevDelayTime = GetTime();

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        // Handle time and determine intent to avoid getting out of sync
        // (may not actally be and issue?)
        const double curTime = GetTime();
        const bool willDrawFrame = (curTime - prevFrameTime) >= frameThreshold;
        const bool willHandleSound = (curTime - prevSoundTime) >= soundThreshold;
        if (willDrawFrame) { prevFrameTime = curTime; }
        if (willHandleSound) { prevSoundTime = curTime; }

        // Handle intent
        if (willDrawFrame) {
            drawScreen(chip8.pixelBuff);
        }
        if (willHandleSound) { handleSound(); }
    }

    CloseWindow();

    return 0;
}
