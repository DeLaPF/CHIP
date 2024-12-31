#ifndef VRAM_H
#define VRAM_H

#include <stdint.h>

typedef struct vram {
    // TODO: one pixel per bit instead of per byte
    uint8_t pixelBuff[8192];

    uint16_t width;
    uint16_t height;
}VRAM;

#endif
