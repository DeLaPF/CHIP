#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

typedef struct display {
    // TODO: need to add support for 128*64
    // as well as one pixel per bit instead of per byte
    uint8_t pixelBuff[2048];  // 64x32

    // apply changes to this buffer, before pushing to pixelBuff
    uint8_t nextPixelBuff[2048];
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
