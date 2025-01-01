#include "vram.h"

uint8_t VRAMGet(VRAM* vram, int ind)
{
    int i = ind / 32;
    int j = ind % 32;
    uint32_t vI = vram->pixelBuff[i];
    uint8_t vIJ = (uint8_t)((vI >> j) & 1);

    return vIJ;
}

void VRAMSet(VRAM* vram, int ind, uint8_t val)
{
    int i = ind / 32;
    int j = ind % 32;
    uint32_t vI = vram->pixelBuff[i];
    uint32_t jMask = 1 << j;
    uint32_t jEraseMask = ~jMask;

    vram->pixelBuff[i] &= jEraseMask;
    if (val) {
        vram->pixelBuff[i] |= jMask;
    }
}
