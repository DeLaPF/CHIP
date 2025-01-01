#ifndef VRAM_H
#define VRAM_H

#include <stddef.h>
#include <stdint.h>

#define VRAM_PIXELS_PER_INDICE 32

typedef struct vram {
    uint32_t* pixelBuff;
    size_t size;
}VRAM;

VRAM makeVRAM(size_t numPixels);
void VRAMZero(VRAM* vram);
void VRAMDestroy(VRAM* vram);

uint8_t VRAMGet(VRAM* vram, int ind);
void VRAMSet(VRAM* vram, int ind, uint8_t val);

#endif
