#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

void clearScreen(uint8_t* pixelBuff);     // 00E0 (clear screen)
void jump(struct cpu* cpu, uint16_t nnn); // 1nnn (jump pc to nnn)
void setVX(struct cpu* cpu, uint8_t x, uint8_t nn);   // 6XNN (set register VX)
void addToVX(struct cpu* cpu, uint8_t x, uint8_t nn); // 7XNN (add value to register VX)
void setIdx(struct cpu* cpu, uint16_t nnn);           // ANNN (set index register I)
void updateBuffer(uint8_t* pixelBuff, struct cpu* cpu, uint8_t x, uint8_t y, uint8_t n); // DXYN (display/draw)

#endif
