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
}
