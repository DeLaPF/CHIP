#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef struct cpu {
    uint8_t registers[16];
    uint16_t idx;
    uint8_t sp;
    uint16_t pc;
}Cpu;

#endif
