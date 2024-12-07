#ifndef MEMORY_MAPPER_H
#define MEMORY_MAPPER_H

#include "display.h"
#include "keyboard.h"
#include "ram.h"


typedef struct memoryMapper {
    RAM* ram;  // 0x0000-0x1000
    Keyboard* kbd;  // 0x1001-0x1002
    Display* scr;  // 0x1003-???
}MemoryMapper;

uint8_t read_u8(uint16_t address);
void write_u8(uint16_t address);
uint16_t read_u16(uint16_t address);
void write_u16(uint16_t address);

#endif
