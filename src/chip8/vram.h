#ifndef VRAM_H
#define VRAM_H

#include <stdint.h>

typedef struct vram {
    uint32_t pixelBuff[256];

    // TODO: maybe it doesn't make the most sense to have this here?
    uint16_t width;
    uint16_t height;
}VRAM;

uint8_t VRAMGet(VRAM* vram, int ind);
void VRAMSet(VRAM* vram, int ind, uint8_t val);

#endif
