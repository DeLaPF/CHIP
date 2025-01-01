#include "sdl_gl.hpp"

extern "C" {
    #include "chip8/chip8.h"
}

void updateKeymap(SDLEventHandler& eH, Chip8& chip8);
