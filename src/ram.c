#include "ram.h"

#include <stdio.h>

#include "constants.h"


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
