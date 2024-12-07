#include "ram.h"

#include <stdio.h>

#include "constants.h"


void loadFont(RAM* ram)
{
    for (int i = 0; i < FONT_BYTES; i++) {
        ram->heap[FONT_START+i] = FONT[i];
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
