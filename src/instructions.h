#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

void clearScreen(uint8_t* pixelBuff);     // 00E0 (clear screen)
void subRet(struct cpu* cpu); // 00EE (return from subroutine)
void jump(struct cpu* cpu, uint16_t nnn); // 1NNN (jump pc to nnn)
void subCall(struct cpu* cpu, uint16_t nnn); // 2NNN (call subroutine at nnn)
void regEqualConst(struct cpu* cpu, uint8_t x, uint16_t nn); // 3XNN (skip instr if vx==nn)
void regNEqualConst(struct cpu* cpu, uint8_t x, uint16_t nn); // 4XNN (skip instr if vx!=nn)
void regEqualReg(struct cpu* cpu, uint8_t x, uint8_t y); // 5XY0 (skip instr if vx==vy)
void regNEqualReg(struct cpu* cpu, uint8_t x, uint8_t y); // 9XY0 (skip instr if vx!=vy)
void setReg(struct cpu* cpu, uint8_t x, uint8_t nn);   // 6XNN (set register VX)
void addToReg(struct cpu* cpu, uint8_t x, uint8_t nn); // 7XNN (add value to register VX)
void setIdx(struct cpu* cpu, uint16_t nnn);           // ANNN (set index register I)
void updateBuffer(uint8_t* pixelBuff, struct cpu* cpu, uint8_t x, uint8_t y, uint8_t n); // DXYN (display/draw)

#endif
