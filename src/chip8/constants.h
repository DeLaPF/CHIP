#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

extern const int CHIP8_BUFF_WIDTH;
extern const int CHIP8_BUFF_HEIGHT;
extern const int SUPER_CHIP_BUFF_WIDTH;
extern const int SUPER_CHIP_BUFF_HEIGHT;

extern const int PROG_START;
extern const int VF;


extern const int LO_FONT_START;
extern const int LO_FONT_BYTES;
extern const int LO_BYTES_PER_CHAR;
extern const uint8_t LO_FONT[80];

extern const int HI_FONT_START;
extern const int HI_FONT_BYTES;
extern const int HI_BYTES_PER_CHAR;
extern const uint8_t HI_FONT[160];

#endif
