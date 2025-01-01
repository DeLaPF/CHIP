#ifndef RAM_H
#define RAM_H

#include <stddef.h>
#include <stdint.h>

#define RAM_STACK_SIZE 16

typedef struct ram {
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint16_t stack[RAM_STACK_SIZE];
    uint8_t* heap;
    size_t heap_s;
}RAM;

RAM makeRAM(size_t heap_s);
void RAMInit(RAM* ram);
void RAMDestroy(RAM* ram);

void loadLoFont(RAM* ram);
void loadHiFont(RAM* ram);

void loadROM(RAM* ram, const char* romPath);

#endif
