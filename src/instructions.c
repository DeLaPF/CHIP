#include <stdint.h>

#include "constants.h"
#include "cpu.h"


void clearScreen(uint8_t* pixelBuff)
{
    for (int i = 0; i < BUFF_WIDTH*BUFF_HEIGHT; i++) {
        pixelBuff[i] = 0;
    }
}

void subRet(struct cpu* cpu)
{
    cpu->sp -= 1; // TODO: throw error if sp < 0
    cpu->pc = cpu->stack[cpu->sp];
}

void jump(struct cpu* cpu, uint16_t nnn)
{
    cpu->pc = nnn;
}

void subCall(struct cpu* cpu, uint16_t nnn)
{
    cpu->stack[cpu->sp] = cpu->pc;
    cpu->pc = nnn;
    cpu->sp += 1;
}

void regEqualConst(struct cpu* cpu, uint8_t x, uint16_t nn)
{
    if (cpu->registers[x] == nn) {
        cpu->pc += 2;
    }
}

void regNEqualConst(struct cpu* cpu, uint8_t x, uint16_t nn)
{
    if (cpu->registers[x] != nn) {
        cpu->pc += 2;
    }
}

void regEqualReg(struct cpu* cpu, uint8_t x, uint8_t y)
{
    if (cpu->registers[x] == cpu->registers[y]) {
        cpu->pc += 2;
    }
}

void regNEqualReg(struct cpu* cpu, uint8_t x, uint8_t y)
{
    if (cpu->registers[x] != cpu->registers[y]) {
        cpu->pc += 2;
    }
}


void setRegToConst(struct cpu* cpu, uint8_t x, uint8_t nn)
{
    cpu->registers[x] = nn;
}

void addConstToReg(struct cpu* cpu, uint8_t x, uint8_t nn)
{
    cpu->registers[x] += nn; // TODO: What happens if overflow?
}

void setRegToReg(struct cpu* cpu, uint8_t x, uint8_t y)
{
    cpu->registers[x] = cpu->registers[y];
}

void setRegORReg(struct cpu* cpu, uint8_t x, uint8_t y)
{
    cpu->registers[x] = cpu->registers[x]|cpu->registers[y];
}

void setRegANDReg(struct cpu* cpu, uint8_t x, uint8_t y)
{
    cpu->registers[x] = cpu->registers[x]&cpu->registers[y];
}

void setRegXORReg(struct cpu* cpu, uint8_t x, uint8_t y)
{
    cpu->registers[x] = cpu->registers[x]^cpu->registers[y];
}

void addRegToReg(struct cpu* cpu, uint8_t x, uint8_t y)
{
    cpu->registers[VF] = (uint16_t)cpu->registers[x]+cpu->registers[y] > 255;
    cpu->registers[x] = cpu->registers[x]+cpu->registers[y];
}

void setRegSubYFromX(struct cpu* cpu, uint8_t x, uint8_t y)
{
    cpu->registers[VF] = cpu->registers[x] >= cpu->registers[y];
    cpu->registers[x] = cpu->registers[x]-cpu->registers[y];
}

void setRegSubXFromY(struct cpu* cpu, uint8_t x, uint8_t y)
{
    cpu->registers[VF] = cpu->registers[y] >= cpu->registers[x];
    cpu->registers[x] = cpu->registers[y]-cpu->registers[x];
}

void setIdx(struct cpu* cpu, uint16_t nnn)
{
    cpu->idx = nnn;
}

void updateBuffer(uint8_t* pixelBuff, struct cpu* cpu, uint8_t x, uint8_t y, uint8_t n)
{
    for (int yOff = 0; yOff < n; yOff++) {
        int wrappedY = (cpu->registers[y] + yOff) % BUFF_HEIGHT;
        uint8_t spriteByte = cpu->heap[cpu->idx+yOff];
        uint8_t byteMask = 0x80;
        for (int xOff = 0; xOff < 8; xOff++) {
            int wrappedX = (cpu->registers[x] + xOff) % BUFF_WIDTH;
            if (spriteByte&byteMask) {
                if (pixelBuff[wrappedY*BUFF_WIDTH+wrappedX]) {
                    cpu->registers[VF] = 1;
                }

                pixelBuff[wrappedY*BUFF_WIDTH+wrappedX] ^= 1;
            }
            byteMask >>= 1;
        }
    }
}
