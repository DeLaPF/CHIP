#include "display.h"

#include <stdio.h>

#include "chip8.h"
#include "constants.h"

#include "raylib.h"


static const int PADDING = 1;
static const int DEBUG_INFO_WIDTH = 30;
static const int DEBUG_INFO_HEIGHT = 54;
static const int DEBUG_BUTTONS_WIDTH = 66;
static const int DEBUG_BUTTONS_HEIGHT = 19;

void updatePixelBuff(Display* scr)
{
    for (int i = 0; i < scr->width*scr->height; i++) {
        scr->pixelBuff[i] = scr->nextPixelBuff[i];
    }
}

void draw(
    void* chip8,
    double curTime,
    double prevCycleTime,
    double prevFrameTime,
    double deltaTime,
    uint16_t opCode,
    uint16_t prevOpCode
)
{
    // TODO: find better solution
    // To avoid circular dependency issue
    Chip8* chip_8 = (Chip8*)chip8;

    const double drawScale = chip_8->scr.width/8.0;
    const double padding = PADDING*drawScale;
    BeginDrawing();
        ClearBackground(GRAY);

        // Draw Buff Frame
        const Vector2 buffFrameOrigin = { padding, padding };
        const double buffFrameWidth = (CHIP8_BUFF_WIDTH+2)*drawScale;
        const double buffFrameHeight = (CHIP8_BUFF_HEIGHT+2)*drawScale;
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
        for (int y = 0; y < chip_8->scr.height; y++) {
            for (int x = 0; x < chip_8->scr.width; x++) {
                if (chip_8->scr.pixelBuff[y*chip_8->scr.width+x]) {
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

        // Draw Debug Info (fps/mspt, registers, stack(ptr), etc.)
        char curTimeBuff[50];
        sprintf(curTimeBuff, "Time: %.2lf", curTime);
        DrawText(
            curTimeBuff,
            debugInfoFrameOrigin.x+padding,
            debugInfoFrameOrigin.y+padding,
            10,
            BLACK
        );
        char deltaTimeBuff[50];
        sprintf(deltaTimeBuff, "Delta: %.5lf", deltaTime);
        DrawText(
            deltaTimeBuff,
            debugInfoFrameOrigin.x+padding,
            debugInfoFrameOrigin.y+padding+10,
            10,
            BLACK
        );
        char prevCycleTimeBuff[50];
        sprintf(prevCycleTimeBuff, "Prev Cycle: %.2lf", prevCycleTime);
        DrawText(
            prevCycleTimeBuff,
            debugInfoFrameOrigin.x+padding,
            debugInfoFrameOrigin.y+padding+20,
            10,
            BLACK
        );
        char prevFrameTimeBuff[50];
        sprintf(prevFrameTimeBuff, "Prev Frame: %.2lf", prevFrameTime);
        DrawText(
            prevFrameTimeBuff,
            debugInfoFrameOrigin.x+padding,
            debugInfoFrameOrigin.y+padding+30,
            10,
            BLACK
        );
        // CPU info
        char buff[200];
        sprintf(
            buff,
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
            chip_8->cpu.registers[0],
            chip_8->cpu.registers[1],
            chip_8->cpu.registers[2],
            chip_8->cpu.registers[3],
            chip_8->cpu.registers[4],
            chip_8->cpu.registers[5],
            chip_8->cpu.registers[6],
            chip_8->cpu.registers[7],
            chip_8->cpu.registers[8],
            chip_8->cpu.registers[9],
            chip_8->cpu.registers[10],
            chip_8->cpu.registers[11],
            chip_8->cpu.registers[12],
            chip_8->cpu.registers[13],
            chip_8->cpu.registers[14],
            chip_8->cpu.registers[15],
            chip_8->cpu.idx,
            chip_8->cpu.pc,
            chip_8->cpu.sp,
            chip_8->cpu.sp ? chip_8->ram.stack[chip_8->cpu.sp-1] : 0x0,
            opCode,
            prevOpCode,
            chip_8->ram.delayTimer,
            chip_8->ram.soundTimer
        );
        DrawText(
            buff,
            debugInfoFrameOrigin.x+padding,
            debugInfoFrameOrigin.y+padding+50,
            10,
            BLACK
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

        // TODO: Draw Debug Buttons (to enable pause/play, step, etc.)
        Vector2 mousePoint = GetMousePosition();
        Rectangle pausePlayBounds = {
            debugButtonsFrameOrigin.x+padding,
            debugButtonsFrameOrigin.y+padding,
            40,
            20
        };
        DrawRectangle(
            pausePlayBounds.x,pausePlayBounds.y,
            pausePlayBounds.width, pausePlayBounds.height,
            GRAY
        );
        DrawText(
            "P/P",
            pausePlayBounds.x+padding, pausePlayBounds.y+padding,
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
            stepBounds.x,stepBounds.y,
            stepBounds.width, stepBounds.height,
            GRAY
        );
        DrawText(
            "Step",
            stepBounds.x+padding, stepBounds.y+padding,
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
            step10Bounds.x,step10Bounds.y,
            step10Bounds.width, step10Bounds.height,
            GRAY
        );
        DrawText(
            "Step 10",
            step10Bounds.x+padding, step10Bounds.y+padding,
            5,
            BLACK
        );
        if (CheckCollisionPointRec(mousePoint, pausePlayBounds)) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                chip_8->isPaused = !chip_8->isPaused;
            }
        }
        if (CheckCollisionPointRec(mousePoint, stepBounds)) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                chip_8->step = 1;
            }
        }
        if (CheckCollisionPointRec(mousePoint, step10Bounds)) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                chip_8->step = 10;
            }
        }

    EndDrawing();
}
