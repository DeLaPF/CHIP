#include <stdint.h>

#include "raylib.h"


const float frameThreshold = 1 / 60.0;
const float soundThreshold = 1 / 60.0;
const float delayThreshold = 1 / 60.0;

struct chip_8 {
    uint8_t registers[16];
    uint8_t frameBuff[2048];
};

void checkered(uint8_t* frameBuff) {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            if (((x%2)+(y%2)+1)%2) {
                frameBuff[y*64+x] = 1;
            }
        }
    }
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

    // Chip-8 screen
    const Vector2 virtualScreenOrigin = { -10.0f, -10.0f };
    const double drawScale = 8.0;
    const double borderWidth = 1.0;
    const Vector2 pixelOrigin = {
        -virtualScreenOrigin.x+(borderWidth*drawScale),
        -virtualScreenOrigin.y+(borderWidth*drawScale),
    };
    Rectangle virtualScreen = {
        0.0f, 0.0f,
        (64.0+(borderWidth*2))*drawScale, (32.0+(borderWidth*2))*drawScale
    };
    struct chip_8 chip8 = {
        { 0 },
        { 0 },
    };
    checkered(chip8.frameBuff);

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
            // draw(&pixelBuff);
            BeginDrawing();
                ClearBackground(GRAY);
                DrawRectanglePro(virtualScreen, virtualScreenOrigin, 0.0f, RAYWHITE);
                for (int y = 0; y < 32; y++) {
                    for (int x = 0; x < 64; x++) {
                        if (chip8.frameBuff[y*64+x]) {
                            DrawRectangle(
                                pixelOrigin.x+(x*drawScale),
                                pixelOrigin.y+(y*drawScale),
                                drawScale,
                                drawScale,
                                BLACK
                            );
                        }
                    }
                }
            EndDrawing();
        }
        if (willHandleSound) { handleSound(); }
    }

    CloseWindow();

    return 0;
}
