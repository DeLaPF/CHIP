#include "keyboard.hpp"

static const SDL_Scancode IND_TO_KEY[16] = {
    SDL_SCANCODE_X,
    SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E,
    SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D,
    SDL_SCANCODE_Z, SDL_SCANCODE_C,
    SDL_SCANCODE_4,
    SDL_SCANCODE_R,
    SDL_SCANCODE_F,
    SDL_SCANCODE_V
};

void updateKeymap(SDLEventHandler* eH, Chip8& chip8)
{
    chip8.keymap = 0;
    for (int i = 0; i <= 0xF; i++) {
        if (eH->isANKeyPressed(IND_TO_KEY[i])) {
            chip8.keymap |= 1 << i;
        }
    }
}
