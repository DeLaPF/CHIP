#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

void clearScreen();  // 00E0 (clear screen)
void setVX();        // 6XNN (set register VX)
void addToVX();      // 7XNN (add value to register VX)
void setIdx();       // ANNN (set index register I)
void updateBuffer(); // DXYN (display/draw)

#endif
