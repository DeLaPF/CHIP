#include <chrono>
#include <iostream>
#include <thread>

#include "glad/gl.h"
#include "SDL.h"

#include "audio.hpp"
#include "bitmap_fb.hpp"
#include "imgui_gl.hpp"
#include "sdl_gl.hpp"
extern "C" {
    #include "chip8/chip8.h"
}

#include "keyboard.hpp"

const int framesPerSecond = 60;
const int defaultCyclesPerFrame = 80;

// TODO: maybe shaders should be hardcoded into the libraries that use them at compile time
// Although it is nice to be able to change things on the fly if necessary
// Maybe define some default shader and a relative search path (if found override default)?
void app(SDL_Window* window, char* romPath)
{
    SDLEventHandler eH(window);
    WaveManager wavM;

    // TODO: raise error if can't load shader instead of just silently aborting
    BitmapFramebuffer bfbLo(64, 32, 400, 400, BitmapFramebuffer::SINGLE_BIT);
    auto bitmapLo = bfbLo.getBitmap();
    BitmapFramebuffer bfbHi(128, 64, 400, 400, BitmapFramebuffer::SINGLE_BIT);
    auto bitmapHi = bfbHi.getBitmap();
    TextureWindow chip8Dislay("CHIP");
    BitmapFramebuffer* curBFB = &bfbLo;

    Chip8 chip8 = makeChip8();
    // Chip8SetVersion(&chip8, Chip8Version::CHIP8);
    // Chip8SetVersion(&chip8, Chip8Version::SCHIP_LEGACY);
    Chip8LoadROM(&chip8, romPath);

    int cyclesPerFrame = defaultCyclesPerFrame;

    // Main loop
    while (!eH.windowShouldClose()) {
        unsigned int frameStart = SDL_GetTicks();
        eH.handleEvents();
        updateKeymap(eH, chip8);
        newImguiFrame();

        // Clear main screen
        glViewport(0, 0, eH.windowW(), eH.windowH());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        bool didDisplayUpdate = false;
        for (int i = 0; i < cyclesPerFrame; i++) {
            // Debug
            if (chip8.isPaused && chip8.step == 0) { break; }
            if (chip8.step > 0) { chip8.step--; }

            Chip8Step(&chip8);

            if (chip8.display.didUpdate) {
                // Enable only drawing when necessary
                didDisplayUpdate = true;

                // Display Wait quirk
                if (chip8.dispWait && !chip8.hiRes) { break; }
            }
        }

        if (chip8.ram.soundTimer) {
            wavM.playWave(4, 10000);
            chip8.ram.soundTimer--;
        } else { wavM.stopSound(); }
        if (chip8.ram.delayTimer) { chip8.ram.delayTimer--; }

        if (didDisplayUpdate) {
            if (chip8.hiRes) {
                curBFB = &bfbHi;
                for (auto i = 0; i < bitmapHi->size(); i++) { bitmapHi->at(i) = chip8.vram.pixelBuff[i]; }
            } else {
                curBFB = &bfbLo;
                for (auto i = 0; i < bitmapLo->size(); i++) { bitmapLo->at(i) = chip8.vram.pixelBuff[i]; }
            }
            curBFB->updateBitmap();
        }
        chip8Dislay.drawWindow(curBFB->getTextureId());
        if (chip8Dislay.didResize()) {
            bfbLo.resizeRenderDim(chip8Dislay.width(), chip8Dislay.height());
            bfbHi.resizeRenderDim(chip8Dislay.width(), chip8Dislay.height());
        }
        curBFB->render();

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

    if (initSDL(SDL_INIT_VIDEO | SDL_INIT_AUDIO, SDL_GL_CONTEXT_DEBUG_FLAG) < 0) { return 1; }
    SDL_Window* window = initSDLGLWindow(800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
