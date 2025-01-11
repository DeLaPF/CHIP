#include <cstdint>

#include "SDL_audio.h"

class WaveManager {
public:
    WaveManager(int sampleDuration);
    ~WaveManager();

    void genWaveSamples(int volume = 5000, int freq = 4, float sampleStepSize = 0.01f);
    // void genWaveSamples(int duration, int volume = 5000, int freq = 4, float sampleStepSize = 0.01f);

    void pauseSample();
    void playSample();
private:
    SDL_AudioDeviceID m_audioDevice;
    SDL_AudioSpec m_audioSpec;

    int m_sampleDuration;
    int16_t* m_samples;
    uint32_t m_numBytes;

    bool m_isPaused;
};
