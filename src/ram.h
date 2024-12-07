#ifndef RAM_H
#define RAM_H

#include <stdint.h>

extern uint16_t DT_PTR;
extern uint16_t ST_PTR;
extern uint16_t FONT_PTR;
extern uint16_t STACK_PTR;
extern uint16_t HEAP_PTR;

typedef union ram {
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t font[80];
    uint16_t stack[16];
    uint8_t heap[3584];
}RAM;

#endif
