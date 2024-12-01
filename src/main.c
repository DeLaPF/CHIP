#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"

#include "constants.h"
#include "cpu.h"
#include "instructions.h"


// const char* ROM_PATH = "./roms/ibm_logo.ch8";
// const char* ROM_PATH = "./roms/bc_test.ch8";
const char* ROM_PATH = "./roms/sc_test.ch8";


const float cycleThreshold = 1 / 700.0;
const float frameThreshold = 1 / 60.0;
const float timerThreshold = 1 / 60.0;

const int PADDING = 1;
const int DEBUG_INFO_WIDTH = 30;
const int DEBUG_INFO_HEIGHT = 54;
const int DEBUG_BUTTONS_WIDTH = 66;
const int DEBUG_BUTTONS_HEIGHT = 19;

struct chip_8 {
    struct cpu *cpu;
    uint8_t pixelBuff[2048];
    bool isPaused;
    uint8_t step;
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

void draw(
    struct chip_8* chip8,
    double curTime,
    double prevCycleTime,
    double prevFrameTime,
    double deltaTime,
    uint16_t opCode
)
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
                if (chip8->pixelBuff[y*BUFF_WIDTH+x]) {
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
        char buff[161];
        sprintf(
            buff,
            "0:%d  1:%d  2:%d  3:%d\n4:%d  5:%d  6:%d  7:%d\n8:%d  9:%d  A:%d  B:%d\nC:%d  D:%d  E:%d  F:%d\nidx:%d  pc:%d\nOP: 0x%4x\nDelay: %d\nSound: %d",
            chip8->cpu->registers[0],
            chip8->cpu->registers[1],
            chip8->cpu->registers[2],
            chip8->cpu->registers[3],
            chip8->cpu->registers[4],
            chip8->cpu->registers[5],
            chip8->cpu->registers[6],
            chip8->cpu->registers[7],
            chip8->cpu->registers[8],
            chip8->cpu->registers[9],
            chip8->cpu->registers[10],
            chip8->cpu->registers[11],
            chip8->cpu->registers[12],
            chip8->cpu->registers[13],
            chip8->cpu->registers[14],
            chip8->cpu->registers[15],
            chip8->cpu->idx,
            chip8->cpu->pc,
            opCode,
            chip8->cpu->delayT,
            chip8->cpu->soundT
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

void handleSound()
{
    // TODO: impl
}

int main(void)
{
    const int windowWidth = 800;
    const int windowHeight = 450;
    InitWindow(windowWidth, windowHeight, "CHIP-8");

    // Init rand
    srand(time(NULL));

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
        true,
    };
    checkered(chip8.pixelBuff);

    printf("Loading Font...\n");
    for (int i = 0; i < FONT_BYTES; i++) {
        chip8.cpu->heap[FONT_START+i] = FONT[i];
    }

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

    // SetTargetFPS(60);
    double prevCycleTime = GetTime();
    double prevFrameTime = GetTime();
    double prevTimerTime = GetTime();

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        bool isPaused = chip8.isPaused && !chip8.step;
        // Handle time and determine intent to avoid getting out of sync
        // (may not actally be and issue?)
        const double curTime = GetTime();
        const bool willCycleCpu = (curTime - prevCycleTime) >= cycleThreshold && !isPaused;
        const bool willDrawFrame = (curTime - prevFrameTime) >= frameThreshold && !isPaused;
        const bool willHandleTimers = (curTime - prevTimerTime) >= timerThreshold && !isPaused;
        if (willCycleCpu) { prevCycleTime = curTime; }
        if (willDrawFrame) { prevFrameTime = curTime; }
        if (willHandleTimers) { prevTimerTime = curTime; }

        // For Debug
        uint16_t opCode = chip8.cpu->heap[chip8.cpu->pc] << 8;
        opCode |= chip8.cpu->heap[chip8.cpu->pc+1];

        // Handle intent
        if (willCycleCpu) {
            if (chip8.step) { chip8.step--; }

            // Fetch
            uint16_t opCode = chip8.cpu->heap[chip8.cpu->pc] << 8;
            opCode |= chip8.cpu->heap[chip8.cpu->pc+1];
            chip8.cpu->pc += 2;

            // Decode and Execute
            uint8_t opNib = (opCode&0xF000) >> 12;
            uint8_t x = (opCode&0x0F00) >> 8;
            uint8_t y = (opCode&0x00F0) >> 4;
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
                            subRet(chip8.cpu);
                            break;
                        default:
                            // TODO: error
                            break;
                    }
                    break;
                case 0x1:
                    jump(chip8.cpu, nnn);
                    break;
                case 0x2:
                    subCall(chip8.cpu, nnn);
                    break;
                case 0x3:
                    regEqualConst(chip8.cpu, x, nn);
                    break;
                case 0x4:
                    regNEqualConst(chip8.cpu, x, nn);
                    break;
                case 0x5:
                    regEqualReg(chip8.cpu, x, y);
                    break;
                case 0x6:
                    setRegToConst(chip8.cpu, x, nn);
                    break;
                case 0x7:
                    addConstToReg(chip8.cpu, x, nn);
                    break;
                case 0x8:
                    switch (n) {
                        case 0x0:
                            setRegToReg(chip8.cpu, x, y);
                            break;
                        case 0x1:
                            setRegORReg(chip8.cpu, x, y);
                            break;
                        case 0x2:
                            setRegANDReg(chip8.cpu, x, y);
                            break;
                        case 0x3:
                            setRegXORReg(chip8.cpu, x, y);
                            break;
                        case 0x4:
                            addRegToReg(chip8.cpu, x, y);
                            break;
                        case 0x5:
                            setRegSubYFromX(chip8.cpu, x, y);
                            break;
                        case 0x6:
                            rightShiftReg(chip8.cpu, x, y, false);
                            break;
                        case 0x7:
                            setRegSubXFromY(chip8.cpu, x, y);
                            break;
                        case 0xE:
                            leftShiftReg(chip8.cpu, x, y, false);
                            break;
                        default:
                            // TODO: error
                            break;
                    }
                    break;
                case 0x9:
                    regNEqualReg(chip8.cpu, x, y);
                    break;
                case 0xA:
                    setIdx(chip8.cpu, nnn);
                    break;
                case 0xB:
                    jumpConstPlusReg(chip8.cpu, x, nnn, false);
                    break;
                case 0xC:
                    setRegConstMaskRand(chip8.cpu, x, nn);
                    break;
                case 0xD:
                    updateBuffer(chip8.pixelBuff, chip8.cpu, x, y, n);
                    break;
                case 0xE:
                    // TODO: handle keys
                    switch(nn) {
                        case 0x9E:
                            skipIfKeyPressed(chip8.cpu, x);
                            break;
                        case 0xA1:
                            skipIfKeyNPressed(chip8.cpu, x);
                            break;
                        default:
                            // TODO: error
                            break;
                    };
                    break;
                case 0xF:
                    switch(nn) {
                        case 0x07:
                            setRegToDelayT(chip8.cpu, x);
                            break;
                        case 0x0A:
                            waitForKeypress(chip8.cpu, x);
                            break;
                        case 0x15:
                            setDelayTToReg(chip8.cpu, x);
                            break;
                        case 0x18:
                            setSoundTToReg(chip8.cpu, x);
                            break;
                        case 0x1E:
                            addRegToIdx(chip8.cpu, x, true);
                            break;
                        case 0x29:
                            setIdxToChar(chip8.cpu, x);
                            break;
                        case 0x33:
                            setHeapIdxToRegDigits(chip8.cpu, x);
                            break;
                        case 0x55:
                            storeRegisters(chip8.cpu, x, false);
                            break;
                        case 0x65:
                            loadMemory(chip8.cpu, x, false);
                            break;
                        default:
                            // TODO: error
                            break;
                    };
                    break;
                default:
                    // TODO: error
                    break;
            };
        }
        if (willDrawFrame) {
            // TODO: update pixelBuff
        }
        if (willHandleTimers) {
            handleSound();
            if (chip8.cpu->delayT) { chip8.cpu->delayT--; }
            if (chip8.cpu->soundT) { chip8.cpu->soundT--; }
        }

        draw(
            &chip8,
            curTime,
            prevCycleTime,
            prevFrameTime,
            GetFrameTime(),
            opCode
        );
    }

    CloseWindow();

    return 0;
}
