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
void setRegToConst(struct cpu* cpu, uint8_t x, uint8_t nn);   // 6XNN (set register VX)
void addConstToReg(struct cpu* cpu, uint8_t x, uint8_t nn); // 7XNN (add value to register VX)
void setRegToReg(struct cpu* cpu, uint8_t x, uint8_t y); // 8XY0 (set vx to vy)
void setRegORReg(struct cpu* cpu, uint8_t x, uint8_t y); // 8XY1 (set vx to vx|vy)
void setRegANDReg(struct cpu* cpu, uint8_t x, uint8_t y); // 8XY2 (set vx to vx&vy)
void setRegXORReg(struct cpu* cpu, uint8_t x, uint8_t y); // 8XY3 (set vx to vx^vy)
void addRegToReg(struct cpu* cpu, uint8_t x, uint8_t y); // 8XY4 (set vx to vx+vy with carry)
void setRegSubYFromX(struct cpu* cpu, uint8_t x, uint8_t y); // 8XY5 (set vx to vx-vy with carry)
void setRegSubXFromY(struct cpu* cpu, uint8_t x, uint8_t y); // 8XY7 (set vx to vy-vx with carry)
void rightShiftReg(struct cpu* cpu, uint8_t x, uint8_t y, bool fromY); // 8XY6 ((opt set vx to vy) right shift vx)
void leftShiftReg(struct cpu* cpu, uint8_t x, uint8_t y, bool fromY); // 8XYE ((opt set vx to vy) left shift vx)
void setIdx(struct cpu* cpu, uint16_t nnn);           // ANNN (set index register I)
void updateBuffer(uint8_t* pixelBuff, struct cpu* cpu, uint8_t x, uint8_t y, uint8_t n); // DXYN (display/draw)

#endif
