#ifndef RAYLIB_DISPLAY_H
#define RAYLIB_DISPLAY_H

#include <stdint.h>

#include "chip8/chip8.h"

void draw(Chip8* chip8, double delta);
void draw_debug(
    Chip8* chip8,
    double curTime,
    double prevCycleTime,
    double prevFrameTime,
    double deltaTime,
    uint16_t opCode,
    uint16_t prevOpCode
);

#endif
