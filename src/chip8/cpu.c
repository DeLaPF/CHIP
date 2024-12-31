#include "cpu.h"

Op peekOp(Cpu* cpu, RAM* ram)
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

Op fetchOp(Cpu* cpu, RAM* ram)
{
    Op op = peekOp(cpu, ram);
    cpu->pc += 2;
    return op;
}
