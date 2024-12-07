#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "chip8.h"

typedef void (*Instruction)(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn);
Instruction decode(uint8_t opNib, uint8_t n, uint8_t nn);

void clearScreen(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn);     // 00E0 (clear screen)
void subRet(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 00EE (return from subroutine)
void jump(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 1NNN (jump pc to nnn)
void subCall(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 2NNN (call subroutine at nnn)
void regEqualConst(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 3XNN (skip instr if vx==nn)
void regNEqualConst(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 4XNN (skip instr if vx!=nn)
void regEqualReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 5XY0 (skip instr if vx==vy)
void regNEqualReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 9XY0 (skip instr if vx!=vy)
void setRegToConst(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn);   // 6XNN (set register VX)
void addConstToReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 7XNN (add value to register VX)
void setRegToReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 8XY0 (set vx to vy)
void setRegORReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 8XY1 (set vx to vx|vy (opt reset vf to 0))
void setRegANDReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 8XY2 (set vx to vx&vy (opt reset vf to 0))
void setRegXORReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 8XY3 (set vx to vx^vy (opt reset vf to 0))
void addRegToReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 8XY4 (set vx to vx+vy with carry)
void setRegSubYFromX(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 8XY5 (set vx to vx-vy with carry)
void setRegSubXFromY(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 8XY7 (set vx to vy-vx with carry)
void rightShiftReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 8XY6 ((opt set vx to vy) right shift vx)
void leftShiftReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // 8XYE ((opt set vx to vy) left shift vx)
void setIdx(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn);           // ANNN (set index register I)
void jumpConstPlusReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // BXNN/BNNN (jump to nnn + vx (or v0 if force0))
void setRegConstMaskRand(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // CXNN (set vx to rand&nn)
void skipIfKeyPressed(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // EX9E (skip instr if key in vx is pressed)
void skipIfKeyNPressed(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // EXA1 (skip instr if key in vx is not pressed)
void waitForKeypress(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // FX0A (don't progress pc until keypress == vx)
void setRegToDelayT(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // FX07 (set vx to delayT)
void setDelayTToReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // FX15 (set delayT to vx)
void setSoundTToReg(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // FX18 (set soundT to vx)
void addRegToIdx(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // FX1E (set idx to idx+vx (opt carry to vf))
void setIdxToChar(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // FX29 (set idx to heap location of char in vx)
void setHeapIdxToRegDigits(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // FX33 (set heap bytes at idx to digit in vx)
void storeRegisters(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // FX55 (store registers v0-vx memory from idx (opt incr idx))
void loadMemory(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // FX65 (load memory from idx into register v0-vx (opt incr idx))
void updateBuffer(Chip8* chip8, uint8_t x, uint8_t y, uint8_t n, uint8_t nn, uint16_t nnn); // DXYN (display/draw)

#endif
