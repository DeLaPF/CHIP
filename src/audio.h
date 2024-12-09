#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>

#include "raylib.h"

typedef struct audio {
    AudioStream stream;
    float audioFrequency;
    bool isAudioDeviceAttached;

    bool isPlaying;
}Audio;

Audio initAudio();
void detachAudio(Audio* audio);
void playAudio(Audio* audio);
void pauseAudio(Audio* audio);

#endif
