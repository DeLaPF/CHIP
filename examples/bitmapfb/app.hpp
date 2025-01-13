#pragma once

#include "BFGe/BFGe.hpp"
#include "BFGe/application.hpp"
#include "BFGe/audio.hpp"
#include "BFGe/bitmap_fb.hpp"
#include "BFGe/imgui_gl.hpp"
extern "C" {
    #include "chip8/chip8.h"
}

namespace CHIPApp {
    class Application : public BFGe::Application {
    public:
        Application(BFGe::Engine*);
        virtual ~Application() override;

        virtual void onInit() override;
        virtual void onTick() override;

    private:
        void updateKeymap(Chip8& chip8);

    private:
        BFGe::Engine* m_engine;
        WaveManager m_wavM;

        Chip8 m_chip8;
        BitmapFramebuffer m_bfbLo;
        std::shared_ptr<Bitmap> m_bitmapLo;
        BitmapFramebuffer m_bfbHi;
        std::shared_ptr<Bitmap> m_bitmapHi;
        TextureWindow m_chip8Dislay;

        BitmapFramebuffer* m_curBFB;
        unsigned int m_prevFrameStart;
        int m_curCyclesPerFrame;
        int m_curTargetFPS;
    };

    const int defaultTargetFramesPerSecond = 60;
    const int defaultCyclesPerFrame = 80;
    const SDL_Scancode IND_TO_KEY[16] = {
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
}
