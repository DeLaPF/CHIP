#ifndef CPU_H
#define CPU_H

struct cpu {
    uint8_t registers[16];
    uint16_t idx;
    uint16_t stack[16];
    uint8_t sp;
    uint16_t pc;
    uint8_t heap[4096];

    uint8_t delayT;
    uint8_t soundT;
};

#endif
