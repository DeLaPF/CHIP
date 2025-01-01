#include <chrono>
#include <iostream>
#include <thread>

#include "glad/gl.h"
#include "SDL.h"

#include "bitmap_fb.hpp"
#include "imgui_gl.hpp"
#include "sdl_gl.hpp"

extern "C" {
    #include "chip8/chip8.h"
}

const int framesPerSecond = 60;
const int defaultCyclesPerFrame = 80;

// TODO: maybe shaders should be hardcoded into the libraries that use them at compile time
// Although it is nice to be able to change things on the fly if necessary
// Maybe define some default shader and a relative search path (if found override default)?
void app(SDL_Window* window, char* romPath)
{
    SDLEventHandler eH(window);
    // TODO: raise error if can't load shader instead of just silently aborting
    BitmapFramebuffer bfb0(64, 32, 400, 400, BitmapFramebuffer::SINGLE_BIT);
    auto bitmap0 = bfb0.getBitmap();
    TextureWindow bfbDisp0("Display 0", bfb0.getTextureId());

    Chip8 chip8 = makeChip8();
    Chip8LoadROM(&chip8, romPath);

    int cyclesPerFrame = defaultCyclesPerFrame;

    // Main loop
    while (!eH.windowShouldClose()) {
        unsigned int frameStart = SDL_GetTicks();
        // TODO: handle kb input
        eH.handleEvents();
        newImguiFrame();

        // Clear main screen
        glViewport(0, 0, eH.windowW(), eH.windowH());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < cyclesPerFrame; i++) {
            if (chip8.isPaused && chip8.step == 0) { break; }
            // TODO: if display wait is enabled, break on draw instruction
            Chip8Step(&chip8);
            if (chip8.step > 0) { chip8.step--; }
        }

        // TODO: handle sound
        if (chip8.ram.delayTimer) { chip8.ram.delayTimer--; }
        if (chip8.ram.soundTimer) { chip8.ram.soundTimer--; }

        // TODO: only update if received display instruction in previous cycles
        // Draw pattern on bitmap(s)
        for (auto i = 0; i < bitmap0->size(); i++) { bitmap0->at(i) = chip8.vram.pixelBuff[i]; }
        bfb0.updateBitmap();

        bfbDisp0.drawWindow();
        if (bfbDisp0.didResize()) { bfb0.resizeRenderDim(bfbDisp0.width(), bfbDisp0.height()); }
        bfb0.render();

        renderImguiFrame();
        SDL_GL_SwapWindow(window);

        unsigned int frameTime = SDL_GetTicks() - frameStart;
        int waitTime = (1000/framesPerSecond) - frameTime;
        if (waitTime > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
        } else if (waitTime < 0) {
            std::cout << "running slow!!! (" << frameTime << "ms per frame)" << std::endl;
        }
    }

    Chip8Destroy(&chip8);
}

int main(int argc, char **argv) {
    // Handle args
    if (argc < 2) {
        std::cout << "Error: specify ROM PATH" << std::endl;
        return 0;
    }
    char* romPath = argv[1];
    bool startPaused = argc > 2 && argv[2][0] == 'p';

    SDL_Window* window = initSDLGLWindow(800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE).value();
    SDL_GLContext glContext = initSDLGLContext(window).value();
    initImguiGL(window, glContext);

    // Init rand to current time
    srand((unsigned int)time(NULL));

    // ensures contents of app are destroyed before glContext
    app(window, romPath);

    destroyImguiGL();
    destroySDLGL(window, glContext);

    return 0;
}
