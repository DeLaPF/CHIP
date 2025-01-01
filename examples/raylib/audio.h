#ifndef RAYLIB_AUDIO_H
#define RAYLIB_AUDIO_H

#include <stdbool.h>

#include "raylib.h"

typedef struct audio {
    AudioStream stream;
    float frequency;
    short data[512];
    short writeBuff[4096];
    bool isAudioDeviceAttached;

    bool isPlaying;
}Audio;

Audio initAudio();
void updateAudio(Audio* audio);
void detachAudio(Audio* audio);
void playAudio(Audio* audio);
void pauseAudio(Audio* audio);

#endif
