#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#include "ram.h"

typedef struct cpu {
    uint8_t registers[16];
    uint16_t idx;
    uint8_t sp;
    uint16_t pc;
}Cpu;

typedef struct op {
    uint16_t code;
    uint16_t nib;
    uint8_t x;
    uint8_t y;
    uint8_t n;
    uint8_t nn;
    uint16_t nnn;
}Op;

Op fetchOp(Cpu* cpu, RAM* ram);

#endif
