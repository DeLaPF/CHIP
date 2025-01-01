#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#include "ram.h"

#define CPU_NUM_REGISTERS 16

typedef struct cpu {
    uint8_t registers[CPU_NUM_REGISTERS];
    uint16_t idx;
    uint16_t pc;
    uint8_t sp;
}CPU;

typedef struct op {
    uint16_t code;
    uint16_t nib;
    uint8_t x;
    uint8_t y;
    uint8_t n;
    uint8_t nn;
    uint16_t nnn;
}Op;

CPU makeCPU();
void CPUInit(CPU* cpu);

Op peekOp(CPU* cpu, RAM* ram);
Op fetchOp(CPU* cpu, RAM* ram);

#endif
