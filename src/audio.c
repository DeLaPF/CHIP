#include "audio.h"

#include <math.h>

#include "raylib.h"

static float frequency = 440.0f;

// Audio frequency, for smoothing
static float audioFrequency = 440.0f;

// Previous value, used to test if sine needs to be rewritten, and to smoothly modulate frequency
static float oldFrequency = 1.0f;

// Index for audio rendering
static float sineIdx = 0.0f;
void AudioInputCallback(void *buffer, unsigned int frames)
{
    audioFrequency = frequency + (audioFrequency - frequency)*0.95f;

    float incr = audioFrequency/44100.0f;
    short *d = (short *)buffer;

    for (unsigned int i = 0; i < frames; i++)
    {
        d[i] = (short)(32000.0f*sinf(2*PI*sineIdx));
        sineIdx += incr;
        if (sineIdx > 1.0f) sineIdx -= 1.0f;
    }
}

Audio initAudio()
{
    SetAudioStreamBufferSizeDefault(4096);
    AudioStream stream = LoadAudioStream(44100, 16, 1);
    bool isAudioDeviceAttached = false;
    if (IsAudioDeviceReady()) {
        SetAudioStreamCallback(stream, AudioInputCallback);
        isAudioDeviceAttached = true;
    }

    Audio audio = {
        .stream=stream,
        .audioFrequency=440.0,
        .isPlaying=false,
        .isAudioDeviceAttached=isAudioDeviceAttached,
    };
    return audio;
}

void detachAudio(Audio* audio)
{
    UnloadAudioStream(audio->stream);
}

void playAudio(Audio* audio)
{
    if (audio->isPlaying) { return; }
    if (audio->isAudioDeviceAttached) {
        PlayAudioStream(audio->stream);
        audio->isPlaying = true;
    }
}

void pauseAudio(Audio* audio)
{
    if (!audio->isPlaying) { return; }
    if (audio->isAudioDeviceAttached) {
        PauseAudioStream(audio->stream);
        audio->isPlaying = false;
    }
}
