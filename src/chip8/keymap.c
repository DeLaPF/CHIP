#include "keymap.h"

bool isKeyDown(Keymap keymap, uint8_t ind)
{
    return keymap & (1 << ind);
}

int getLowestKeyPressed(Keymap keymap)
{
    uint8_t ind = 0;
    while (keymap) {
        if (keymap&1) { return ind; }
        keymap >>= 1;
        ind++;
    }
    return -1;
}
