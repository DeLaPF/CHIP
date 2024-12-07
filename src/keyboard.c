#include "keyboard.h"

#include "raylib.h"

static const uint8_t IND_TO_KEY[16] = {
    KEY_X,
    KEY_ONE, KEY_TWO, KEY_THREE,
    KEY_Q, KEY_W, KEY_E,
    KEY_A, KEY_S, KEY_D,
    KEY_Z, KEY_C,
    KEY_FOUR,
    KEY_R,
    KEY_F,
    KEY_V
};

void updateKeyMap(Keyboard* kbd)
{
    kbd->keyMap = 0;
    for (int i = 0; i <= 0xF; i++) {
        if (IsKeyDown(IND_TO_KEY[i])) {
            kbd->keyMap |= 1 << i;
        }
    }
}

bool isKeyDown(Keyboard* kbd, uint8_t ind)
{
    return kbd->keyMap & (1 << ind);
}

int getLowestKeyPressed(Keyboard* kbd)
{
    uint16_t keyMap = kbd->keyMap;
    uint8_t ind = 0;
    while (keyMap) {
        if (keyMap&1) { return ind; }
        ind++;
        keyMap >>= 1;
    }
    return -1;
}
