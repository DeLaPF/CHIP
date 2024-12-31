#include "display.h"

#include <stdio.h>

#include "chip8/chip8.h"
#include "chip8/constants.h"

#include "raylib.h"

static const int PADDING = 1;
static const int DEBUG_INFO_WIDTH = 30;
static const int DEBUG_INFO_HEIGHT = 54;
static const int DEBUG_BUTTONS_WIDTH = 66;
static const int DEBUG_BUTTONS_HEIGHT = 19;

typedef struct ivec2 {
    int x;
    int y;
}iVec2;

void draw(Chip8* chip8, double delta)
{
    const int drawScale = 12;
    const int buffPixelSize = (int)(chip8->hiRes ? drawScale/2 : drawScale);
    const int padding = (int)(PADDING*drawScale);
    BeginDrawing();
        ClearBackground(GRAY);

        // Draw Buff Frame
        const iVec2 buffFrameOrigin = { padding, padding };
        const int buffFrameWidth = (CHIP8_BUFF_WIDTH+2)*drawScale;
        const int buffFrameHeight = (CHIP8_BUFF_HEIGHT+2)*drawScale;
        DrawRectangle(
            buffFrameOrigin.x, buffFrameOrigin.y,
            buffFrameWidth, buffFrameHeight,
            RAYWHITE
        );
        const iVec2 pixelBuffOrigin = {
            buffFrameOrigin.x+padding,
            buffFrameOrigin.y+padding
        };
        for (int y = 0; y < chip8->vram.height; y++) {
            for (int x = 0; x < chip8->vram.width; x++) {
                if (chip8->vram.pixelBuff[y*chip8->vram.width+x]) {
                    DrawRectangle(
                        pixelBuffOrigin.x+(x*buffPixelSize),
                        pixelBuffOrigin.y+(y*buffPixelSize),
                        buffPixelSize,
                        buffPixelSize,
                        BLACK
                    );
                }
            }
        }

        // Draw mspt
        char deltaBuff[50];
        snprintf(deltaBuff, sizeof(deltaBuff), "Time: %.8lf", delta);
        DrawText(deltaBuff, padding, padding, 20, BLACK);
    EndDrawing();
}

void draw_debug(
    Chip8* chip8,
    double curTime,
    double prevCycleTime,
    double prevFrameTime,
    double deltaTime,
    uint16_t opCode,
    uint16_t prevOpCode
)
{
    const int drawScale = 8;
    const int buffPixelSize = (int)(chip8->hiRes ? drawScale/2 : drawScale);
    const int padding = (int)(PADDING*drawScale);
    BeginDrawing();
        ClearBackground(GRAY);

        // Draw Buff Frame
        const iVec2 buffFrameOrigin = { padding, padding };
        const int buffFrameWidth = (CHIP8_BUFF_WIDTH+2)*drawScale;
        const int buffFrameHeight = (CHIP8_BUFF_HEIGHT+2)*drawScale;
        DrawRectangle(
            buffFrameOrigin.x, buffFrameOrigin.y,
            buffFrameWidth, buffFrameHeight,
            RAYWHITE
        );

        // Draw Pixel Buffer to Buff Frame
        const iVec2 pixelBuffOrigin = {
            buffFrameOrigin.x+padding,
            buffFrameOrigin.y+padding
        };
        for (int y = 0; y < chip8->vram.height; y++) {
            for (int x = 0; x < chip8->vram.width; x++) {
                if (chip8->vram.pixelBuff[y*chip8->vram.width+x]) {
                    DrawRectangle(
                        pixelBuffOrigin.x+(x*buffPixelSize),
                        pixelBuffOrigin.y+(y*buffPixelSize),
                        buffPixelSize,
                        buffPixelSize,
                        BLACK
                    );
                }
            }
        }

        // Draw Debug Info Frame
        const iVec2 debugInfoFrameOrigin = {
            buffFrameOrigin.x+buffFrameWidth+padding,
            padding
        };
        const int debugInfoFrameHeight = DEBUG_INFO_HEIGHT*drawScale;
        const int debugInfoFrameWidth = DEBUG_INFO_WIDTH*drawScale;
        DrawRectangle(
            debugInfoFrameOrigin.x, debugInfoFrameOrigin.y,
            debugInfoFrameWidth, debugInfoFrameHeight,
            RAYWHITE
        );

        // Draw Debug Info (fps/mspt, registers, stack(ptr), etc.)
        char curTimeBuff[50];
        snprintf(curTimeBuff, sizeof(curTimeBuff), "Time: %.2lf", curTime);
        DrawText(
            curTimeBuff,
            debugInfoFrameOrigin.x+padding,
            debugInfoFrameOrigin.y+padding,
            10,
            BLACK
        );
        char deltaTimeBuff[50];
        snprintf(deltaTimeBuff, sizeof(deltaTimeBuff), "Delta: %.5lf", deltaTime);
        DrawText(
            deltaTimeBuff,
            debugInfoFrameOrigin.x+padding,
            debugInfoFrameOrigin.y+padding+10,
            10,
            BLACK
        );
        char prevCycleTimeBuff[50];
        snprintf(prevCycleTimeBuff, sizeof(prevCycleTimeBuff), "Prev Cycle: %.2lf", prevCycleTime);
        DrawText(
            prevCycleTimeBuff,
            debugInfoFrameOrigin.x+padding,
            debugInfoFrameOrigin.y+padding+20,
            10,
            BLACK
        );
        char prevFrameTimeBuff[50];
        snprintf(prevFrameTimeBuff, sizeof(prevFrameTimeBuff), "Prev Frame: %.2lf", prevFrameTime);
        DrawText(
            prevFrameTimeBuff,
            debugInfoFrameOrigin.x+padding,
            debugInfoFrameOrigin.y+padding+30,
            10,
            BLACK
        );
        // CPU info
        char buff[200];
        snprintf(
            buff,
            sizeof(buff),
            "0:%x  1:%x  2:%x  3:%x\n"
            "4:%x  5:%x  6:%x  7:%x\n"
            "8:%x  9:%x  A:%x  B:%x\n"
            "C:%x  D:%x  E:%x  F:%x\n"
            "idx:%x\n"
            "pc:%x\n"
            "sp:%x\n"
            "ret:%x\n"
            "OP: 0x%4x\n"
            "Prev OP: 0x%4x\n"
            "Delay: %d\nSound: %d",
            chip8->cpu.registers[0],
            chip8->cpu.registers[1],
            chip8->cpu.registers[2],
            chip8->cpu.registers[3],
            chip8->cpu.registers[4],
            chip8->cpu.registers[5],
            chip8->cpu.registers[6],
            chip8->cpu.registers[7],
            chip8->cpu.registers[8],
            chip8->cpu.registers[9],
            chip8->cpu.registers[10],
            chip8->cpu.registers[11],
            chip8->cpu.registers[12],
            chip8->cpu.registers[13],
            chip8->cpu.registers[14],
            chip8->cpu.registers[15],
            chip8->cpu.idx,
            chip8->cpu.pc,
            chip8->cpu.sp,
            chip8->cpu.sp ? chip8->ram.stack[chip8->cpu.sp-1] : 0x0,
            opCode,
            prevOpCode,
            chip8->ram.delayTimer,
            chip8->ram.soundTimer
        );
        DrawText(
            buff,
            debugInfoFrameOrigin.x+padding,
            debugInfoFrameOrigin.y+padding+50,
            10,
            BLACK
        );

        // Draw Debug Buttons Frame
        Rectangle debugButtonsFrame = {
            (float)padding,
            (float)buffFrameOrigin.y+buffFrameHeight+padding,
            (float)DEBUG_BUTTONS_WIDTH*drawScale,
            (float)DEBUG_BUTTONS_HEIGHT*drawScale,
        };
        DrawRectangle(
            (int)debugButtonsFrame.x, (int)debugButtonsFrame.y,
            (int)debugButtonsFrame.width, (int)debugButtonsFrame.height,
            RAYWHITE
        );

        // TODO: Draw Debug Buttons (to enable pause/play, step, etc.)
        Vector2 mousePoint = GetMousePosition();
        Rectangle pausePlayBounds = {
            debugButtonsFrame.x+padding,
            debugButtonsFrame.y+padding,
            40,
            20
        };
        DrawRectangle(
            (int)pausePlayBounds.x,(int)pausePlayBounds.y,
            (int)pausePlayBounds.width, (int)pausePlayBounds.height,
            GRAY
        );
        DrawText(
            "P/P",
            (int)pausePlayBounds.x+padding, (int)pausePlayBounds.y+padding,
            5,
            BLACK
        );
        Rectangle stepBounds = {
            pausePlayBounds.x,
            pausePlayBounds.y+pausePlayBounds.height+padding,
            40,
            20
        };
        DrawRectangle(
            (int)stepBounds.x, (int)stepBounds.y,
            (int)stepBounds.width, (int)stepBounds.height,
            GRAY
        );
        DrawText(
            "Step",
            (int)stepBounds.x+padding, (int)stepBounds.y+padding,
            5,
            BLACK
        );
        Rectangle step10Bounds = {
            stepBounds.x,
            stepBounds.y+stepBounds.height+padding,
            40,
            20
        };
        DrawRectangle(
            (int)step10Bounds.x, (int)step10Bounds.y,
            (int)step10Bounds.width, (int)step10Bounds.height,
            GRAY
        );
        DrawText(
            "Step 10",
            (int)step10Bounds.x+padding, (int)step10Bounds.y+padding,
            5,
            BLACK
        );
        if (CheckCollisionPointRec(mousePoint, pausePlayBounds)) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                chip8->isPaused = !chip8->isPaused;
            }
        }
        if (CheckCollisionPointRec(mousePoint, stepBounds)) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                chip8->step = 1;
            }
        }
        if (CheckCollisionPointRec(mousePoint, step10Bounds)) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                chip8->step = 10;
            }
        }

    EndDrawing();
}
