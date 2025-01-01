#include "instructions.h"

#include <stdlib.h>

#include "constants.h"

void clearScreen(Chip8* chip8, Op* op)
{
    for (int i = 0; i < chip8->display.width*chip8->display.height; i++) {
        VRAMSet(&chip8->vram, i, 0);
    }
}

void subRet(Chip8* chip8, Op* op)
{
    chip8->cpu.sp -= 1; // TODO: throw error if sp < 0
    chip8->cpu.pc = chip8->ram.stack[chip8->cpu.sp];
}

void jump(Chip8* chip8, Op* op)
{
    chip8->cpu.pc = op->nnn;
}

void subCall(Chip8* chip8, Op* op)
{
    chip8->ram.stack[chip8->cpu.sp] = chip8->cpu.pc;
    chip8->cpu.pc = op->nnn;
    chip8->cpu.sp += 1;
}

void regEqualConst(Chip8* chip8, Op* op)
{
    if (chip8->cpu.registers[op->x] == op->nn) {
        chip8->cpu.pc += 2;
    }
}

void regNEqualConst(Chip8* chip8, Op* op)
{
    if (chip8->cpu.registers[op->x] != op->nn) {
        chip8->cpu.pc += 2;
    }
}

void regEqualReg(Chip8* chip8, Op* op)
{
    if (chip8->cpu.registers[op->x] == chip8->cpu.registers[op->y]) {
        chip8->cpu.pc += 2;
    }
}

void regNEqualReg(Chip8* chip8, Op* op)
{
    if (chip8->cpu.registers[op->x] != chip8->cpu.registers[op->y]) {
        chip8->cpu.pc += 2;
    }
}


void setRegToConst(Chip8* chip8, Op* op)
{
    chip8->cpu.registers[op->x] = op->nn;
}

void addConstToReg(Chip8* chip8, Op* op)
{
    chip8->cpu.registers[op->x] += op->nn; // TODO: What happens if overflow?
}

void setRegToReg(Chip8* chip8, Op* op)
{
    chip8->cpu.registers[op->x] = chip8->cpu.registers[op->y];
}

void setRegORReg(Chip8* chip8, Op* op)
{
    chip8->cpu.registers[op->x] = chip8->cpu.registers[op->x]|chip8->cpu.registers[op->y];
    if (chip8->resetVF) { chip8->cpu.registers[VF] = 0; }
}

void setRegANDReg(Chip8* chip8, Op* op)
{
    chip8->cpu.registers[op->x] = chip8->cpu.registers[op->x]&chip8->cpu.registers[op->y];
    if (chip8->resetVF) { chip8->cpu.registers[VF] = 0; }
}

void setRegXORReg(Chip8* chip8, Op* op)
{
    chip8->cpu.registers[op->x] = chip8->cpu.registers[op->x]^chip8->cpu.registers[op->y];
    if (chip8->resetVF) { chip8->cpu.registers[VF] = 0; }
}

void addRegToReg(Chip8* chip8, Op* op)
{
    uint8_t vx = chip8->cpu.registers[op->x];
    chip8->cpu.registers[op->x] = chip8->cpu.registers[op->x]+chip8->cpu.registers[op->y];
    chip8->cpu.registers[VF] = (uint16_t)vx+chip8->cpu.registers[op->y] > 255;
}

void setRegSubYFromX(Chip8* chip8, Op* op)
{
    uint8_t vx = chip8->cpu.registers[op->x];
    chip8->cpu.registers[op->x] = chip8->cpu.registers[op->x]-chip8->cpu.registers[op->y];
    chip8->cpu.registers[VF] = vx >= chip8->cpu.registers[op->y];
}

void setRegSubXFromY(Chip8* chip8, Op* op)
{
    uint8_t vx = chip8->cpu.registers[op->x];
    chip8->cpu.registers[op->x] = chip8->cpu.registers[op->y]-chip8->cpu.registers[op->x];
    chip8->cpu.registers[VF] = chip8->cpu.registers[op->y] >= vx;
}

void rightShiftReg(Chip8* chip8, Op* op)
{
    if (chip8->fromY) { chip8->cpu.registers[op->x] = chip8->cpu.registers[op->y]; }
    uint8_t vx = chip8->cpu.registers[op->x];
    chip8->cpu.registers[op->x] = chip8->cpu.registers[op->x] >> 1;
    chip8->cpu.registers[VF] = vx&0x1;
}

void leftShiftReg(Chip8* chip8, Op* op)
{
    if (chip8->fromY) { chip8->cpu.registers[op->x] = chip8->cpu.registers[op->y]; }
    uint8_t vx = chip8->cpu.registers[op->x];
    chip8->cpu.registers[op->x] = chip8->cpu.registers[op->x] << 1;
    chip8->cpu.registers[VF] = (vx&0x80) >> 7;
}

void setIdxToConst(Chip8* chip8, Op* op)
{
    chip8->cpu.idx = op->nnn;
}

void jumpConstPlusReg(Chip8* chip8, Op* op)
{
    if (chip8->force0) { op->x = 0; }
    chip8->cpu.pc = op->nnn+chip8->cpu.registers[op->x];
}

void setRegConstMaskRand(Chip8* chip8, Op* op)
{
    uint8_t r = rand() % 255;
    chip8->cpu.registers[op->x] = r&op->nn;
}

void skipIfKeyPressed(Chip8* chip8, Op* op)
{
    if (isKeyDown(chip8->keymap, chip8->cpu.registers[op->x])) {
        chip8->cpu.pc += 2;
    }
}

void skipIfKeyNPressed(Chip8* chip8, Op* op)
{
    if (!isKeyDown(chip8->keymap, chip8->cpu.registers[op->x])) {
        chip8->cpu.pc += 2;
    }
}

static int keyInd = -1;
void waitForKeypress(Chip8* chip8, Op* op)
{
    if (keyInd < 0) {
        keyInd = getLowestKeyPressed(chip8->keymap);
    }

    if (keyInd < 0 || isKeyDown(chip8->keymap, keyInd)) {
        chip8->cpu.pc -= 2;
    } else {
        chip8->cpu.registers[op->x] = keyInd;
        keyInd = -1;
    }
}

void setRegToDelayT(Chip8* chip8, Op* op)
{
    chip8->cpu.registers[op->x] = chip8->ram.delayTimer;
}

void setDelayTToReg(Chip8* chip8, Op* op)
{
     chip8->ram.delayTimer = chip8->cpu.registers[op->x];
}

void setSoundTToReg(Chip8* chip8, Op* op)
{
     chip8->ram.soundTimer = chip8->cpu.registers[op->x];
}

void addRegToIdx(Chip8* chip8, Op* op)
{
    uint16_t idx = chip8->cpu.idx;
    chip8->cpu.idx = idx+chip8->cpu.registers[op->x];
    if (chip8->carry) {
        chip8->cpu.registers[VF] = idx+chip8->cpu.registers[op->x] > 0xFFFF;
    }
}

void setIdxToChar(Chip8* chip8, Op* op)
{
    chip8->cpu.idx = (chip8->cpu.registers[op->x]*LO_BYTES_PER_CHAR)+LO_FONT_START;
}

void storeRegDigits(Chip8* chip8, Op* op)
{
    uint8_t val = chip8->cpu.registers[op->x];
    chip8->ram.heap[chip8->cpu.idx+2] = val%10;
    val /= 10;
    chip8->ram.heap[chip8->cpu.idx+1] = val%10;
    val /= 10;
    chip8->ram.heap[chip8->cpu.idx] = val%10;
}

void storeRegisters(Chip8* chip8, Op* op)
{
    for (int i = 0; i <= op->x; i++) {
        chip8->ram.heap[chip8->cpu.idx+i] = chip8->cpu.registers[i];
    }
    if (chip8->increment) { chip8->cpu.idx += op->x+1; }
}

void loadMemory(Chip8* chip8, Op* op)
{
    for (int i = 0; i <= op->x; i++) {
        chip8->cpu.registers[i] = chip8->ram.heap[chip8->cpu.idx+i];
    }
    if (chip8->increment) { chip8->cpu.idx += op->x+1; }
}

void updateBuffer(Chip8* chip8, Op* op)
{
    uint8_t vx = chip8->cpu.registers[op->x]%chip8->display.width;
    uint8_t vy = chip8->cpu.registers[op->y]%chip8->display.height;
    chip8->cpu.registers[VF] = 0;

    // SuperChip
    bool hiResSprite = chip8->hiRes && op->n == 0;
    if (hiResSprite) { op->n = 16; }

    for (int yOff = 0; yOff < op->n; yOff++) {
        int py = vy + yOff;
        if (chip8->clipping) {
            if (py < 0 || py >= chip8->display.height) { continue; }
        } else {
            py %= chip8->display.height;
        }

        uint16_t spriteRow = chip8->ram.heap[chip8->cpu.idx+yOff];
        uint16_t rowMask = 0x80;
        uint8_t rowWidth = 8;
        if (hiResSprite) {
            spriteRow = chip8->ram.heap[chip8->cpu.idx+(yOff*2)] << 8;
            spriteRow |= chip8->ram.heap[chip8->cpu.idx+(yOff*2)+1];
            rowMask = 0x8000;
            rowWidth = 16;
        }

        for (int xOff = 0; xOff < rowWidth; xOff++) {
            int px = vx + xOff;
            if (chip8->clipping) {
                if (px < 0 || px >= chip8->display.width) { continue; }
            } else {
                px %= chip8->display.width;
            }

            int pInd = py*chip8->display.width+px;
            if (spriteRow&rowMask) {
                uint8_t pVal = VRAMGet(&chip8->vram, pInd);
                if (pVal) {
                    chip8->cpu.registers[VF] = 1;
                }

                VRAMSet(&chip8->vram, pInd, pVal^1);
            }
            rowMask >>= 1;
        }
    }
}

void scrollDN(Chip8* chip8, Op* op)
{
    int shift = op->n;
    for (int py = chip8->display.height-1; py >= 0; py--) {
        for (int px = 0; px < chip8->display.width; px++) {
            int pInd = py*chip8->display.width+px;
            if (py > shift) {
                int pShiftInd = (py-shift)*chip8->display.width+px;
                VRAMSet(&chip8->vram, pInd, VRAMGet(&chip8->vram, pShiftInd));
            } else {
                VRAMSet(&chip8->vram, pInd, 0);
            }
        }
    }
}

void scrollR(Chip8* chip8, Op* op)
{
    int shift = 4;
    for (int py = 0; py < chip8->display.height; py++) {
        for (int px = chip8->display.width-1; px >= 0; px--) {
            int pInd = py*chip8->display.width+px;
            if (px > shift) {
                VRAMSet(&chip8->vram, pInd, VRAMGet(&chip8->vram, pInd-shift));
            } else {
                VRAMSet(&chip8->vram, pInd, 0);
            }
        }
    }
}

void scrollL(Chip8* chip8, Op* op)
{
    int shift = 4;
    for (int py = 0; py < chip8->display.height; py++) {
        for (int px = 0; px < chip8->display.width; px++) {
            int pInd = py*chip8->display.width+px;
            if (px < chip8->display.width-shift) {
                VRAMSet(&chip8->vram, pInd, VRAMGet(&chip8->vram, pInd+shift));
            } else {
                VRAMSet(&chip8->vram, pInd, 0);
            }
        }
    }
}

void loRes(Chip8* chip8, Op* op)
{
    chip8->hiRes = false;
    chip8->display.width = CHIP8_BUFF_WIDTH;
    chip8->display.height = CHIP8_BUFF_HEIGHT;
}

void hiRes(Chip8* chip8, Op* op)
{
    chip8->hiRes = true;
    chip8->display.width = SUPER_CHIP_BUFF_WIDTH;
    chip8->display.height = SUPER_CHIP_BUFF_HEIGHT;
}

void setIdxToHiChar(Chip8* chip8, Op* op)
{
    chip8->cpu.idx = (chip8->cpu.registers[op->x]*HI_BYTES_PER_CHAR)+HI_FONT_START;
}

void saveToFlags(Chip8* chip8, Op* op)
{
    for (int i = 0; i <= op->x; i++) {
        chip8->flagRegisters[i] = chip8->cpu.registers[i];
    }
}

void loadFromFlags(Chip8* chip8, Op* op)
{
    for (int i = 0; i <= op->x; i++) {
        chip8->cpu.registers[i] = chip8->flagRegisters[i];
    }
}

Instruction decode(Op* op)
{
    switch (op->nib) {
    case 0x0:
        if (op->y == 0xC) {
            return scrollDN;
        }
        switch (op->nn) {
        case 0xE0:
            return clearScreen;
        case 0xEE:
            return subRet;
        case 0xFB:
            return scrollR;
        case 0xFC:
            return scrollL;
        case 0xFE:
            return loRes;
        case 0xFF:
            return hiRes;
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
        switch (op->n) {
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
        return setIdxToConst;
    case 0xB:
        return jumpConstPlusReg;
    case 0xC:
        return setRegConstMaskRand;
    case 0xD:
        return updateBuffer;
    case 0xE:
        switch(op->nn) {
        case 0x9E:
            return skipIfKeyPressed;
        case 0xA1:
            return skipIfKeyNPressed;
        default:
            return 0;
        };
    case 0xF:
        switch(op->nn) {
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
        case 0x30:
            return setIdxToHiChar;
        case 0x33:
            return storeRegDigits;
        case 0x55:
            return storeRegisters;
        case 0x65:
            return loadMemory;
        case 0x75:
            return saveToFlags;
        case 0x85:
            return loadFromFlags;
        default:
            return 0;
        };
    default:
        return 0;
    };
}
