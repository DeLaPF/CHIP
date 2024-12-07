#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>


typedef struct keyboard {
    uint16_t keyMap;
}Keyboard;

void updateKeyMap(Keyboard* kbd);

#endif
