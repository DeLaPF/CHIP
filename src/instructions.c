#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "raylib.h"

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

void rightShiftReg(struct cpu* cpu, uint8_t x, uint8_t y, bool fromY)
{
    if (fromY) { cpu->registers[x] = cpu->registers[y]; }
    cpu->registers[VF] = cpu->registers[x]&0x1;
    cpu->registers[x] = cpu->registers[x] >> 1;
}

void leftShiftReg(struct cpu* cpu, uint8_t x, uint8_t y, bool fromY)
{
    if (fromY) { cpu->registers[x] = cpu->registers[y]; }
    cpu->registers[VF] = (cpu->registers[x]&0x80) >> 7;
    cpu->registers[x] = cpu->registers[x] << 1;
}

void setIdx(struct cpu* cpu, uint16_t nnn)
{
    cpu->idx = nnn;
}

void jumpConstPlusReg(struct cpu* cpu, uint8_t x, uint16_t nnn, bool force0)
{
    if (force0) { x = 0; }
    cpu->pc = nnn+cpu->registers[x];
}

void setRegConstMaskRand(struct cpu* cpu, uint8_t x, uint8_t nn)
{
    uint8_t r = rand() % 255;
    cpu->registers[x] = r&nn;
}

void skipIfKeyPressed(struct cpu* cpu, uint8_t x)
{
    if (IsKeyDown(IND_TO_KEY[cpu->registers[x]])) {
        cpu->pc += 2;
    }
}

void skipIfKeyNPressed(struct cpu* cpu, uint8_t x)
{
    if (!IsKeyDown(IND_TO_KEY[cpu->registers[x]])) {
        cpu->pc += 2;
    }
}

void waitForKeypress(struct cpu* cpu, uint8_t x)
{
    if (!IsKeyDown(IND_TO_KEY[cpu->registers[x]])) {
        cpu->pc -= 2;
    }
}

void setRegToDelayT(struct cpu* cpu, uint8_t x)
{
    cpu->registers[x] = cpu->delayT;
}

void setDelayTToReg(struct cpu* cpu, uint8_t x)
{
     cpu->delayT = cpu->registers[x];
}

void setSoundTToReg(struct cpu* cpu, uint8_t x)
{
     cpu->soundT = cpu->registers[x];
}

void addRegToIdx(struct cpu* cpu, uint8_t x, bool carry)
{
    if (carry) {
        cpu->registers[VF] = (uint16_t)cpu->registers[x]+cpu->idx > 0xFFFF;
    }
    cpu->idx += (uint16_t)cpu->registers[x];
}

void setIdxToChar(struct cpu* cpu, uint8_t x)
{
    cpu->idx = (cpu->registers[x]*BYTES_PER_CHAR)+FONT_START;
}

void setHeapIdxToRegDigits(struct cpu* cpu, uint8_t x)
{
    uint8_t val = cpu->registers[x];
    cpu->heap[cpu->idx+2] = val%10;
    val /= 10;
    cpu->heap[cpu->idx+1] = val%10;
    val /= 10;
    cpu->heap[cpu->idx] = val%10;
}

void storeRegisters(struct cpu* cpu, uint8_t x, bool increment)
{
    for (int i = 0; i <= x; i++) {
        cpu->heap[cpu->idx+i] = cpu->registers[i];
    }
    if (increment) { cpu->idx += x+1; }
}

void loadMemory(struct cpu* cpu, uint8_t x, bool increment)
{
    for (int i = 0; i <= x; i++) {
        cpu->registers[i] = cpu->heap[cpu->idx+i];
    }
    if (increment) { cpu->idx += x+1; }
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
