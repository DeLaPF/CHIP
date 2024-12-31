#ifndef KEYMAP_H
#define KEYMAP_H

#include <stdbool.h>
#include <stdint.h>

typedef uint16_t Keymap;

bool isKeyDown(Keymap keymap, uint8_t ind);
int getLowestKeyPressed(Keymap keymap);

#endif
