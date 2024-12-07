#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>


typedef struct keyboard {
    uint16_t keyMap;
}Keyboard;

void updateKeyMap(Keyboard* kbd);
bool isKeyDown(Keyboard* kbd, uint8_t ind);
int getLowestKeyPressed(Keyboard* kbd);

#endif
