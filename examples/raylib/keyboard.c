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

void updateKeyMap(Chip8* chip8)
{
    chip8->keymap = 0;
    for (int i = 0; i <= 0xF; i++) {
        if (IsKeyDown(IND_TO_KEY[i])) {
            chip8->keymap |= 1 << i;
        }
    }
}
