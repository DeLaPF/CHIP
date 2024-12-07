#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>


typedef struct display {
    // TODO: need to add support for 128*64
    // as well as one pixel per bit instead of per byte
    uint8_t pixelBuff[2048];  // 64x32
}Display;

#endif
