#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "chip8/chip8.h"

typedef void (*Instruction)(Chip8* chip8, Op* op);
Instruction decode(Op* op);

// Chip8
void clearScreen(Chip8* chip8, Op* op);         // 00E0 (clear screen)
void subRet(Chip8* chip8, Op* op);              // 00EE (return from subroutine)
void jump(Chip8* chip8, Op* op);                // 1NNN (jump pc to nnn)
void subCall(Chip8* chip8, Op* op);             // 2NNN (call subroutine at nnn)
void regEqualConst(Chip8* chip8, Op* op);       // 3XNN (skip instr if vx==nn)
void regNEqualConst(Chip8* chip8, Op* op);      // 4XNN (skip instr if vx!=nn)
void regEqualReg(Chip8* chip8, Op* op);         // 5XY0 (skip instr if vx==vy)
void regNEqualReg(Chip8* chip8, Op* op);        // 9XY0 (skip instr if vx!=vy)
void setRegToConst(Chip8* chip8, Op* op);       // 6XNN (set register VX)
void addConstToReg(Chip8* chip8, Op* op);       // 7XNN (add value to register VX)
void setRegToReg(Chip8* chip8, Op* op);         // 8XY0 (set vx to vy)
void setRegORReg(Chip8* chip8, Op* op);         // 8XY1 (set vx to vx|vy)
void setRegANDReg(Chip8* chip8, Op* op);        // 8XY2 (set vx to vx&vy)
void setRegXORReg(Chip8* chip8, Op* op);        // 8XY3 (set vx to vx^vy)
void addRegToReg(Chip8* chip8, Op* op);         // 8XY4 (set vx to vx+vy with carry)
void setRegSubYFromX(Chip8* chip8, Op* op);     // 8XY5 (set vx to vx-vy with carry)
void setRegSubXFromY(Chip8* chip8, Op* op);     // 8XY7 (set vx to vy-vx with carry)
void rightShiftReg(Chip8* chip8, Op* op);       // 8XY6 (right shift vx)
void leftShiftReg(Chip8* chip8, Op* op);        // 8XYE (left shift vx)
void setIdxToConst(Chip8* chip8, Op* op);       // ANNN (set idx to const)
void jumpConstPlusReg(Chip8* chip8, Op* op);    // BXNN/BNNN (jump to nnn + vx)
void setRegConstMaskRand(Chip8* chip8, Op* op); // CXNN (set vx to rand&nn)
void skipIfKeyPressed(Chip8* chip8, Op* op);    // EX9E (skip instr if key in vx is pressed)
void skipIfKeyNPressed(Chip8* chip8, Op* op);   // EXA1 (skip instr if key in vx is not pressed)
void waitForKeypress(Chip8* chip8, Op* op);     // FX0A (don't progress pc until keypress == vx)
void setRegToDelayT(Chip8* chip8, Op* op);      // FX07 (set vx to delayT)
void setDelayTToReg(Chip8* chip8, Op* op);      // FX15 (set delayT to vx)
void setSoundTToReg(Chip8* chip8, Op* op);      // FX18 (set soundT to vx)
void addRegToIdx(Chip8* chip8, Op* op);         // FX1E (set idx to idx+vx)
void setIdxToChar(Chip8* chip8, Op* op);        // FX29 (set idx to heap location of char in vx)
void storeRegDigits(Chip8* chip8, Op* op);      // FX33 (store digits in vx to memory at idx+(0-2))
void storeRegisters(Chip8* chip8, Op* op);      // FX55 (store registers v0-vx to memory at idx)
void loadMemory(Chip8* chip8, Op* op);          // FX65 (load memory from idx into register v0-vx)
void updateBuffer(Chip8* chip8, Op* op);        // DXYN (display/draw)

// SuperChip
void scrollDN(Chip8* chip8, Op* op);            // 00CN (scroll down by n px)
void scrollR(Chip8* chip8, Op* op);             // 00FB (scroll right by 4 px)
void scrollL(Chip8* chip8, Op* op);             // 00FC (scroll left by 4 px)
void loRes(Chip8* chip8, Op* op);               // 00FE (low res mode (disable hi res))
void hiRes(Chip8* chip8, Op* op);               // 00FF (high res mode)
void setIdxToHiChar(Chip8* chip8, Op* op);      // FX30 (set idx to heap location of hi res char in vx)
void saveToFlags(Chip8* chip8, Op* op);         // FX75 (save registers v0-vx to flag registers)
void loadFromFlags(Chip8* chip8, Op* op);       // FX85 (load flag registers into register v0-vx)

#endif
