#ifndef RAM_H
#define RAM_H

#include <stdint.h>

typedef struct ram {
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint16_t stack[16];
    uint8_t heap[4096];
}RAM;

void loadLoFont(RAM* ram);
void loadHiFont(RAM* ram);

void loadROM(RAM* ram, const char* romPath);

void RAMInit(RAM* ram);

#endif
