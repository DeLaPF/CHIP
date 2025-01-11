#include "audio.hpp"

#include <cmath>
#include <iostream>

WaveManager::WaveManager(int sampleDuration)
    : m_audioSpec{}
{
    SDL_zero(this->m_audioSpec);
    this->m_audioSpec.freq = 44100;
    this->m_audioSpec.format = AUDIO_S16SYS;
    this->m_audioSpec.channels = 1;
    this->m_audioSpec.samples = 1024;
    this->m_audioSpec.callback = NULL;


    std::cout << SDL_GetNumAudioDevices(0) << std::endl;
    this->m_audioDevice = SDL_OpenAudioDevice(NULL, 0, &this->m_audioSpec, NULL, 0);
    if (!this->m_audioDevice) {
        std::cout << "Error [SDL_AUDIO]: could not open device" << std::endl;
    }

    this->m_isPaused = true;
    this->m_sampleDuration = sampleDuration;
    this->m_numBytes = this->m_audioSpec.freq * this->m_sampleDuration * sizeof(int16_t);
    this->m_samples = (int16_t*)malloc(this->m_numBytes);
}

WaveManager::~WaveManager() {
    SDL_CloseAudioDevice(this->m_audioDevice);
    free(this->m_samples);
}

void WaveManager::genWaveSamples(int volume, int freq, float sampleStepSize)
{
    float x = 0.0f;
    for (int i = 0; i < this->m_audioSpec.freq * this->m_sampleDuration; i++) {
        this->m_samples[i] = (int16_t)(sin(x * freq) * volume);
        x += sampleStepSize;
    }
    SDL_QueueAudio(this->m_audioDevice, this->m_samples, this->m_numBytes);
}

void WaveManager::pauseSample()
{
    if (this->m_isPaused) { return; }
    SDL_PauseAudioDevice(this->m_audioDevice, 1);
    this->m_isPaused = true;
}

// TODO: requeue sample if buffer has run out
void WaveManager::playSample()
{
    // if (!this->m_isPaused) { return; }
    SDL_PauseAudioDevice(this->m_audioDevice, 0);
    this->m_isPaused = false;
}
