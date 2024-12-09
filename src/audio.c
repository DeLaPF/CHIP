#include "audio.h"

static void generateSamples(void* buffer, unsigned int frames)
{
    short* samples = (short*)buffer;
    for (unsigned int i = 0; i < frames; i++) { samples[i] = 440; }
}

Audio initAudio()
{
    SetAudioStreamBufferSizeDefault(4096);
    AudioStream stream = LoadAudioStream(44100, 16, 1);
    // AttachAudioStreamProcessor(stream, generateSamples);
    // PlayAudioStream(stream);

    Audio audio = {
        .stream=stream,
        .audioFrequency=440.0,
        .isPlaying=false,
    };
    return audio;
}

void detachAudio(Audio* audio)
{
    UnloadAudioStream(audio->stream);
    // DetachAudioStreamProcessor(audio->stream, generateSamples);
}

void playAudio(Audio* audio)
{
    if (audio->isPlaying) { return; }

    // PlayAudioStream(audio->stream);
}

void pauseAudio(Audio* audio)
{
    if (!audio->isPlaying) { return; }

    // PauseAudioStream(audio->stream);
}
