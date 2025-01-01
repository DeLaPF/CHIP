#include "cpu.h"

#include <string.h>

CPU makeCPU()
{
    CPU cpu = {
        .registers={0},
        .idx=0,
        .sp=0,
        .pc=0,
    };
    CPUInit(&cpu);

    return cpu;
}

void CPUInit(CPU* cpu)
{
    memset(cpu->registers, 0 , CPU_NUM_REGISTERS);
    cpu->idx = 0;
    cpu->sp = 0;
    cpu->pc = 0;
}

Op peekOp(CPU* cpu, RAM* ram)
{
    uint16_t code = ram->heap[cpu->pc] << 8;
    code |= ram->heap[cpu->pc+1];

    Op op = {
        .code=code,
        .nib=(code&0xF000) >> 12,
        .x=(code&0x0F00) >> 8,
        .y=(code&0x00F0) >> 4,
        .n=code&0x000F,
        .nn=code&0x00FF,
        .nnn=code&0x0FFF,
    };
    return op;
}

Op fetchOp(CPU* cpu, RAM* ram)
{
    Op op = peekOp(cpu, ram);
    cpu->pc += 2;
    return op;
}
