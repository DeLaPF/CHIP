#include "vram.h"

#include <stdlib.h>
#include <string.h>

VRAM makeVRAM(size_t numPixels)
{
    size_t numIndices = numPixels/VRAM_PIXELS_PER_INDICE + 1;
    VRAM vram = {
        .pixelBuff=(uint32_t*)malloc(numIndices*sizeof(uint32_t)),
        .size=numIndices
    };

    return vram;
}

void VRAMZero(VRAM* vram)
{
    memset(vram->pixelBuff, 0, vram->size*sizeof(uint32_t));
}

void VRAMDestroy(VRAM* vram)
{
    free(vram->pixelBuff);
}

uint8_t VRAMGet(VRAM* vram, int ind)
{
    int i = ind / VRAM_PIXELS_PER_INDICE;
    int j = ind % VRAM_PIXELS_PER_INDICE;
    uint32_t vI = vram->pixelBuff[i];
    uint8_t vIJ = (uint8_t)((vI >> j) & 1);

    return vIJ;
}

void VRAMSet(VRAM* vram, int ind, uint8_t val)
{
    int i = ind / VRAM_PIXELS_PER_INDICE;
    int j = ind % VRAM_PIXELS_PER_INDICE;
    uint32_t vI = vram->pixelBuff[i];
    uint32_t jMask = 1 << j;
    uint32_t jEraseMask = ~jMask;

    vram->pixelBuff[i] &= jEraseMask;
    if (val) {
        vram->pixelBuff[i] |= jMask;
    }
}
