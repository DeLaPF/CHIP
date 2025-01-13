#include "app.hpp"

#include "BFGe/entrypoint.hpp"
#include "BFGe/sdl_gl.hpp"

#include <chrono>
#include <format>
#include <iostream>
#include <stdexcept>
#include <thread>

#include "keyboard.hpp"

// TODO: maybe shaders should be hardcoded into the libraries that use them at compile time
// Although it is nice to be able to change things on the fly if necessary
// Maybe define some default shader and a relative search path (if found override default)?
CHIPApp::Application::Application(BFGe::Engine* engine):
    // TODO: raise error if can't load shader instead of just silently aborting
    m_bfbLo(64, 32, 400, 400, BitmapFramebuffer::SINGLE_BIT),
    m_bfbHi(128, 64, 400, 400, BitmapFramebuffer::SINGLE_BIT),
    m_chip8Dislay("CHIP")
{
    this->m_engine = engine;
    this->m_chip8 = makeChip8();
    this->m_curBFB = &this->m_bfbLo;
    this->m_bitmapLo = this->m_bfbLo.getBitmap();
    this->m_bitmapHi = this->m_bfbHi.getBitmap();
    this->m_prevFrameStart = 0;
}

CHIPApp::Application::~Application()
{
    Chip8Destroy(&this->m_chip8);
}

void CHIPApp::Application::onInit() {
    int argc = this->m_engine->getArgC();
    char** argv = this->m_engine->getArgV();

    // Handle args
    if (argc < 2) { std::runtime_error("Error: specify ROM PATH"); }
    char* romPath = argv[1];
    bool startPaused = argc > 2 && argv[2][0] == 'p';

    // Chip8SetVersion(&chip8, Chip8Version::CHIP8);
    // Chip8SetVersion(&chip8, Chip8Version::SCHIP_LEGACY);
    Chip8LoadROM(&this->m_chip8, romPath);

    this->m_curCyclesPerFrame = defaultCyclesPerFrame;
    this->m_curTargetFPS = CHIPApp::defaultTargetFramesPerSecond;
}

void CHIPApp::Application::onTick()
{
    // Handle frame timing to maintain target fps
    unsigned int frameTime = SDL_GetTicks() - this->m_prevFrameStart;
    int targetFrameTime = (1000/this->m_curTargetFPS);
    int waitTime = targetFrameTime - frameTime;
    if (waitTime > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
    } else if (waitTime < 0) {
        std::cout << std::format(
            "Running slow!!! Last frame: {}ms (Target: {}ms)",
            frameTime,
            targetFrameTime
        ) << std::endl;
    }
    this->m_prevFrameStart = SDL_GetTicks();

    // CHIP8 core
    updateKeymap(this->m_engine->getEventHandler(), this->m_chip8);
    bool didDisplayUpdate = false;
    for (int i = 0; i < this->m_curCyclesPerFrame; i++) {
        // Debug
        if (this->m_chip8.isPaused && this->m_chip8.step == 0) { break; }
        if (this->m_chip8.step > 0) { this->m_chip8.step--; }

        Chip8Step(&this->m_chip8);

        if (this->m_chip8.display.didUpdate) {
            // Enable only drawing when necessary
            didDisplayUpdate = true;

            // Display Wait quirk
            if (this->m_chip8.dispWait && !this->m_chip8.hiRes) { break; }
        }
    }

    // Handle timers
    if (this->m_chip8.ram.soundTimer) {
        this->m_wavM.playWave(4, 10000);
        this->m_chip8.ram.soundTimer--;
    } else { this->m_wavM.stopSound(); }
    if (this->m_chip8.ram.delayTimer) { this->m_chip8.ram.delayTimer--; }

    // Handle updating/rendering BmpFb(s)
    if (didDisplayUpdate) {
        if (this->m_chip8.hiRes) {
            this->m_curBFB = &this->m_bfbHi;
            for (auto i = 0; i < this->m_bitmapHi->size(); i++) {
                this->m_bitmapHi->at(i) = this->m_chip8.vram.pixelBuff[i];
            }
        } else {
            this->m_curBFB = &this->m_bfbLo;
            for (auto i = 0; i < this->m_bitmapLo->size(); i++) {
                this->m_bitmapLo->at(i) = this->m_chip8.vram.pixelBuff[i];
            }
        }
        this->m_curBFB->updateBitmap();
    }
    this->m_chip8Dislay.drawWindow(this->m_curBFB->getTextureId());
    if (this->m_chip8Dislay.didResize()) {
        this->m_bfbLo.resizeRenderDim(this->m_chip8Dislay.width(), this->m_chip8Dislay.height());
        this->m_bfbHi.resizeRenderDim(this->m_chip8Dislay.width(), this->m_chip8Dislay.height());
    }
    this->m_curBFB->render();
}

// Defines the application that will be ran by the engine
BFGe::Application* BFGe::CreateApplication(BFGe::Engine* engine)
{
    initSDLSubsystem(SDL_INIT_AUDIO);

    // Init rand to current time
    srand((unsigned int)time(NULL));

    return new CHIPApp::Application(engine);
}
