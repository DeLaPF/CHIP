#include "instructions.h"

#include <stdlib.h>

#include "constants.h"
#include "keyboard.h"


void clearScreen(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    for (int i = 0; i < BUFF_WIDTH*BUFF_HEIGHT; i++) {
        chip8->scr->pixelBuff[i] = 0;
    }
}

void subRet(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    chip8->cpu->sp -= 1; // TODO: throw error if sp < 0
    chip8->cpu->pc = chip8->ram->stack[chip8->cpu->sp];
}

void jump(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    chip8->cpu->pc = nnn;
}

void subCall(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    chip8->ram->stack[chip8->cpu->sp] = chip8->cpu->pc;
    chip8->cpu->pc = nnn;
    chip8->cpu->sp += 1;
}

void regEqualConst(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    if (chip8->cpu->registers[x] == nn) {
        chip8->cpu->pc += 2;
    }
}

void regNEqualConst(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    if (chip8->cpu->registers[x] != nn) {
        chip8->cpu->pc += 2;
    }
}

void regEqualReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    if (chip8->cpu->registers[x] == chip8->cpu->registers[y]) {
        chip8->cpu->pc += 2;
    }
}

void regNEqualReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    if (chip8->cpu->registers[x] != chip8->cpu->registers[y]) {
        chip8->cpu->pc += 2;
    }
}


void setRegToConst(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    chip8->cpu->registers[x] = nn;
}

void addConstToReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    chip8->cpu->registers[x] += nn; // TODO: What happens if overflow?
}

void setRegToReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    chip8->cpu->registers[x] = chip8->cpu->registers[y];
}

void setRegORReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    chip8->cpu->registers[x] = chip8->cpu->registers[x]|chip8->cpu->registers[y];
    if (chip8->resetVF) { chip8->cpu->registers[VF] = 0; }
}

void setRegANDReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    chip8->cpu->registers[x] = chip8->cpu->registers[x]&chip8->cpu->registers[y];
    if (chip8->resetVF) { chip8->cpu->registers[VF] = 0; }
}

void setRegXORReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    chip8->cpu->registers[x] = chip8->cpu->registers[x]^chip8->cpu->registers[y];
    if (chip8->resetVF) { chip8->cpu->registers[VF] = 0; }
}

void addRegToReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    uint8_t vx = chip8->cpu->registers[x];
    chip8->cpu->registers[x] = chip8->cpu->registers[x]+chip8->cpu->registers[y];
    chip8->cpu->registers[VF] = (uint16_t)vx+chip8->cpu->registers[y] > 255;
}

void setRegSubYFromX(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    uint8_t vx = chip8->cpu->registers[x];
    chip8->cpu->registers[x] = chip8->cpu->registers[x]-chip8->cpu->registers[y];
    chip8->cpu->registers[VF] = vx >= chip8->cpu->registers[y];
}

void setRegSubXFromY(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    uint8_t vx = chip8->cpu->registers[x];
    chip8->cpu->registers[x] = chip8->cpu->registers[y]-chip8->cpu->registers[x];
    chip8->cpu->registers[VF] = chip8->cpu->registers[y] >= vx;
}

void rightShiftReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    if (chip8->fromY) { chip8->cpu->registers[x] = chip8->cpu->registers[y]; }
    uint8_t vx = chip8->cpu->registers[x];
    chip8->cpu->registers[x] = chip8->cpu->registers[x] >> 1;
    chip8->cpu->registers[VF] = vx&0x1;
}

void leftShiftReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    if (chip8->fromY) { chip8->cpu->registers[x] = chip8->cpu->registers[y]; }
    uint8_t vx = chip8->cpu->registers[x];
    chip8->cpu->registers[x] = chip8->cpu->registers[x] << 1;
    chip8->cpu->registers[VF] = (vx&0x80) >> 7;
}

void setIdx(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    chip8->cpu->idx = nnn;
}

void jumpConstPlusReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    if (chip8->force0) { x = 0; }
    chip8->cpu->pc = nnn+chip8->cpu->registers[x];
}

void setRegConstMaskRand(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    uint8_t r = rand() % 255;
    chip8->cpu->registers[x] = r&nn;
}

void skipIfKeyPressed(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    if (isKeyDown(chip8->kbd, chip8->cpu->registers[x])) {
        chip8->cpu->pc += 2;
    }
}

void skipIfKeyNPressed(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    if (!isKeyDown(chip8->kbd, chip8->cpu->registers[x])) {
        chip8->cpu->pc += 2;
    }
}

static int keyInd = -1;
void waitForKeypress(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    if (keyInd < 0) {
        keyInd = getLowestKeyPressed(chip8->kbd);
    }

    if (keyInd < 0 || isKeyDown(chip8->kbd, keyInd)) {
        chip8->cpu->pc -= 2;
    } else {
        chip8->cpu->registers[x] = keyInd;
        keyInd = -1;
    }
}

void setRegToDelayT(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    chip8->cpu->registers[x] = chip8->ram->delayTimer;
}

void setDelayTToReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
     chip8->ram->delayTimer = chip8->cpu->registers[x];
}

void setSoundTToReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
     chip8->ram->soundTimer = chip8->cpu->registers[x];
}

void addRegToIdx(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    uint16_t idx = chip8->cpu->idx;
    chip8->cpu->idx = idx+chip8->cpu->registers[x];
    if (chip8->carry) {
        chip8->cpu->registers[VF] = idx+chip8->cpu->registers[x] > 0xFFFF;
    }
}

void setIdxToChar(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    chip8->cpu->idx = (chip8->cpu->registers[x]*BYTES_PER_CHAR)+FONT_START;
}

void setHeapIdxToRegDigits(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    uint8_t val = chip8->cpu->registers[x];
    chip8->ram->heap[chip8->cpu->idx+2] = val%10;
    val /= 10;
    chip8->ram->heap[chip8->cpu->idx+1] = val%10;
    val /= 10;
    chip8->ram->heap[chip8->cpu->idx] = val%10;
}

void storeRegisters(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    for (int i = 0; i <= x; i++) {
        chip8->ram->heap[chip8->cpu->idx+i] = chip8->cpu->registers[i];
    }
    if (chip8->increment) { chip8->cpu->idx += x+1; }
}

void loadMemory(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    for (int i = 0; i <= x; i++) {
        chip8->cpu->registers[i] = chip8->ram->heap[chip8->cpu->idx+i];
    }
    if (chip8->increment) { chip8->cpu->idx += x+1; }
}

void updateBuffer(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn)
{
    uint8_t vx = chip8->cpu->registers[x]%BUFF_WIDTH;
    uint8_t vy = chip8->cpu->registers[y]%BUFF_HEIGHT;

    for (int yOff = 0; yOff < n; yOff++) {
        int py = vy + yOff;
        if (py < 0 || py >= BUFF_HEIGHT) { continue; }
        uint8_t spriteByte = chip8->ram->heap[chip8->cpu->idx+yOff];
        uint8_t byteMask = 0x80;
        for (int xOff = 0; xOff < 8; xOff++) {
            int px = vx + xOff;
            if (px < 0 || px >= BUFF_WIDTH) { continue; }
            int pInd = py*BUFF_WIDTH+px;
            if (spriteByte&byteMask) {
                if (chip8->scr->pixelBuff[pInd]) {
                    chip8->cpu->registers[VF] = 1;
                }

                chip8->scr->pixelBuff[pInd] ^= 1;
            }
            byteMask >>= 1;
        }
    }
}

Instruction decode(uint8_t opNib, uint8_t n, uint8_t nn)
{
    switch (opNib) {
    case 0x0:
        switch (nn) {
        case 0xE0:
            return clearScreen;
        case 0xEE:
            return subRet;
        default:
            return 0;
        }
    case 0x1:
        return jump;
    case 0x2:
        return subCall;
    case 0x3:
        return regEqualConst;
    case 0x4:
        return regNEqualConst;
    case 0x5:
        return regEqualReg;
    case 0x6:
        return setRegToConst;
    case 0x7:
        return addConstToReg;
    case 0x8:
        switch (n) {
        case 0x0:
            return setRegToReg;
        case 0x1:
            return setRegORReg;
        case 0x2:
            return setRegANDReg;
        case 0x3:
            return setRegXORReg;
        case 0x4:
            return addRegToReg;
        case 0x5:
            return setRegSubYFromX;
        case 0x6:
            return rightShiftReg;
        case 0x7:
            return setRegSubXFromY;
        case 0xE:
            return leftShiftReg;
        default:
            return 0;
        }
    case 0x9:
        return regNEqualReg;
    case 0xA:
        return setIdx;
    case 0xB:
        return jumpConstPlusReg;
    case 0xC:
        return setRegConstMaskRand;
    case 0xD:
        return updateBuffer;
    case 0xE:
        switch(nn) {
        case 0x9E:
            return skipIfKeyPressed;
        case 0xA1:
            return skipIfKeyNPressed;
        default:
            return 0;
        };
    case 0xF:
        switch(nn) {
        case 0x07:
            return setRegToDelayT;
        case 0x0A:
            return waitForKeypress;
        case 0x15:
            return setDelayTToReg;
        case 0x18:
            return setSoundTToReg;
        case 0x1E:
            return addRegToIdx;
        case 0x29:
            return setIdxToChar;
        case 0x33:
            return setHeapIdxToRegDigits;
        case 0x55:
            return storeRegisters;
        case 0x65:
            return loadMemory;
        default:
            return 0;
        };
    default:
        // printf("Error Unknown Instruction (Very Bad): 0x%x\n", opCode);
        return 0;
    };
}
