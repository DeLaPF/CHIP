#include "audio.h"

#include <math.h>
#include <string.h>

#include "raylib.h"

#define MAX_SAMPLES               512
#define MAX_SAMPLES_PER_UPDATE   4096

Audio initAudio()
{
    SetAudioStreamBufferSizeDefault(4096);
    AudioStream stream = LoadAudioStream(44100, 16, 1);
    Audio audio = {
        .stream=stream,
        .frequency=440.0,
        .data={0},
        .writeBuff={0},
        .isPlaying=false,
        .isAudioDeviceAttached=IsAudioDeviceReady(),
    };
    updateAudio(&audio);
    int waveLength = (int)(22050/audio.frequency);
    if (waveLength > MAX_SAMPLES/2) waveLength = MAX_SAMPLES/2;
    if (waveLength < 1) waveLength = 1;

    // Write sine wave
    for (int i = 0; i < waveLength*2; i++)
    {
        audio.data[i] = (short)(sinf(((2*PI*(float)i/waveLength)))*32000);
    }
    // Make sure the rest of the line is flat
    for (int j = waveLength*2; j < MAX_SAMPLES; j++) {
        audio.data[j] = (short)0;
    }

    return audio;
}

void updateAudio(Audio* audio)
{
    if (audio->isAudioDeviceAttached && IsAudioStreamProcessed(audio->stream)) {
        int waveLength = (int)(22050/audio->frequency);
        int writeCursor = 0;
        int readCursor = 0;
        while (writeCursor < MAX_SAMPLES_PER_UPDATE) {
            // Start by trying to write the whole chunk at once
            int writeLength = MAX_SAMPLES_PER_UPDATE-writeCursor;

            // Limit to the maximum readable size
            int readLength = waveLength-readCursor;

            if (writeLength > readLength) writeLength = readLength;

            // Write the slice
            memcpy(audio->writeBuff + writeCursor, audio->data + readCursor, writeLength*sizeof(short));

            // Update cursors and loop audio
            readCursor = (readCursor + writeLength) % waveLength;

            writeCursor += writeLength;
        }

        UpdateAudioStream(audio->stream, audio->writeBuff, MAX_SAMPLES_PER_UPDATE);
    }
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
