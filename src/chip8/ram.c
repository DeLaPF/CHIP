#include "ram.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"

RAM makeRAM(size_t heap_s)
{
    RAM ram = {
        .delayTimer=0,
        .soundTimer=0,
        .stack={0},
        .heap=(uint8_t*)malloc(heap_s),
        .heap_s=heap_s
    };
    RAMInit(&ram);

    return ram;
}

void RAMInit(RAM* ram)
{
    memset(ram->heap, 0, ram->heap_s);
    loadLoFont(ram);
    loadHiFont(ram);

    ram->delayTimer=0;
    ram->soundTimer=0;
    memset(ram->stack, 0, RAM_STACK_SIZE*sizeof(uint16_t));
}

void RAMDestroy(RAM* ram)
{
    free(ram->heap);
}

void loadLoFont(RAM* ram)
{
    for (int i = 0; i < LO_FONT_BYTES; i++) {
        ram->heap[LO_FONT_START+i] = LO_FONT[i];
    }
}

void loadHiFont(RAM* ram)
{
    for (int i = 0; i < HI_FONT_BYTES; i++) {
        ram->heap[HI_FONT_START+i] = HI_FONT[i];
    }
}

void loadROM(RAM* ram, const char* romPath)
{
    uint8_t* buffer = &ram->heap[PROG_START];
    FILE* fileptr = fopen(romPath, "rb");
    fseek(fileptr, 0, SEEK_END);
    long filelen = ftell(fileptr);
    rewind(fileptr);
    fread(buffer, filelen, 1, fileptr);
    fclose(fileptr);
}
