#ifndef VRAM_H
#define VRAM_H

#include <stdint.h>

typedef struct vram {
    uint32_t pixelBuff[256];
}VRAM;

uint8_t VRAMGet(VRAM* vram, int ind);
void VRAMSet(VRAM* vram, int ind, uint8_t val);

#endif
