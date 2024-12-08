#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

typedef struct display {
    // TODO: one pixel per bit instead of per byte
    uint8_t pixelBuff[8192];

    // apply changes to this buffer, before pushing to pixelBuff
    uint8_t nextPixelBuff[8192];

    uint16_t width;
    uint16_t height;
}Display;

void updatePixelBuff(Display* scr);
void draw(
    void* chip8,
    double curTime,
    double prevCycleTime,
    double prevFrameTime,
    double deltaTime,
    uint16_t opCode,
    uint16_t prevOpCode
);

#endif
