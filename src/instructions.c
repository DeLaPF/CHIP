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

void setRegORReg(struct cpu* cpu, uint8_t x, uint8_t y, bool resetVF)
{
    cpu->registers[x] = cpu->registers[x]|cpu->registers[y];
    if (resetVF) { cpu->registers[VF] = 0; }
}

void setRegANDReg(struct cpu* cpu, uint8_t x, uint8_t y, bool resetVF)
{
    cpu->registers[x] = cpu->registers[x]&cpu->registers[y];
    if (resetVF) { cpu->registers[VF] = 0; }
}

void setRegXORReg(struct cpu* cpu, uint8_t x, uint8_t y, bool resetVF)
{
    cpu->registers[x] = cpu->registers[x]^cpu->registers[y];
    if (resetVF) { cpu->registers[VF] = 0; }
}

void addRegToReg(struct cpu* cpu, uint8_t x, uint8_t y)
{
    uint8_t vx = cpu->registers[x];
    cpu->registers[x] = cpu->registers[x]+cpu->registers[y];
    cpu->registers[VF] = (uint16_t)vx+cpu->registers[y] > 255;
}

void setRegSubYFromX(struct cpu* cpu, uint8_t x, uint8_t y)
{
    uint8_t vx = cpu->registers[x];
    cpu->registers[x] = cpu->registers[x]-cpu->registers[y];
    cpu->registers[VF] = vx >= cpu->registers[y];
}

void setRegSubXFromY(struct cpu* cpu, uint8_t x, uint8_t y)
{
    uint8_t vx = cpu->registers[x];
    cpu->registers[x] = cpu->registers[y]-cpu->registers[x];
    cpu->registers[VF] = cpu->registers[y] >= vx;
}

void rightShiftReg(struct cpu* cpu, uint8_t x, uint8_t y, bool fromY)
{
    if (fromY) { cpu->registers[x] = cpu->registers[y]; }
    uint8_t vx = cpu->registers[x];
    cpu->registers[x] = cpu->registers[x] >> 1;
    cpu->registers[VF] = vx&0x1;
}

void leftShiftReg(struct cpu* cpu, uint8_t x, uint8_t y, bool fromY)
{
    if (fromY) { cpu->registers[x] = cpu->registers[y]; }
    uint8_t vx = cpu->registers[x];
    cpu->registers[x] = cpu->registers[x] << 1;
    cpu->registers[VF] = (vx&0x80) >> 7;
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

static int keyInd = -1;
void waitForKeypress(struct cpu* cpu, uint8_t x)
{
    if (keyInd < 0) {
        for (int i = 0; i <= 0xF; i++) {
            if (IsKeyDown(IND_TO_KEY[i])) {
                keyInd = i;
            }
        }
    }

    if (keyInd < 0 || IsKeyDown(IND_TO_KEY[keyInd])) {
        cpu->pc -= 2;
    } else {
        cpu->registers[x] = keyInd;
        keyInd = -1;
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
    uint16_t idx = cpu->idx;
    cpu->idx = idx+cpu->registers[x];
    if (carry) {
        cpu->registers[VF] = idx+cpu->registers[x] > 0xFFFF;
    }
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
    uint8_t vx = cpu->registers[x]%BUFF_WIDTH;
    uint8_t vy = cpu->registers[y]%BUFF_HEIGHT;

    for (int yOff = 0; yOff < n; yOff++) {
        int py = vy + yOff;
        if (py < 0 || py >= BUFF_HEIGHT) { continue; }
        uint8_t spriteByte = cpu->heap[cpu->idx+yOff];
        uint8_t byteMask = 0x80;
        for (int xOff = 0; xOff < 8; xOff++) {
            int px = vx + xOff;
            if (px < 0 || px >= BUFF_WIDTH) { continue; }
            int pInd = py*BUFF_WIDTH+px;
            if (spriteByte&byteMask) {
                if (pixelBuff[pInd]) {
                    cpu->registers[VF] = 1;
                }

                pixelBuff[pInd] ^= 1;
            }
            byteMask >>= 1;
        }
    }
}
