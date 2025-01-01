#include "vram.h"

uint8_t VRAMGet(VRAM* vram, int ind)
{
    return vram->pixelBuff[ind];
}

void VRAMSet(VRAM* vram, int ind, uint8_t val)
{
    vram->pixelBuff[ind] = val;
}
