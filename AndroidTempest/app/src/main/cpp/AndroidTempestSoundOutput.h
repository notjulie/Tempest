//
// Created by Randy Rasmussen on 1/14/24.
//

#ifndef ANDROIDTEMPEST_ANDROIDTEMPESTSOUNDOUTPUT_H
#define ANDROIDTEMPEST_ANDROIDTEMPESTSOUNDOUTPUT_H


#include <mutex>
#include <stdint.h>
#include "AbstractSoundGenerator.h"
#include "Cpp11WaveStreamer.h"
#include "SoundChannelState.h"
#include "WaveSoundDriver.h"

class AndroidTempestSoundOutput : public AbstractSoundGenerator, private WaveSoundDriver
{
public:
    AndroidTempestSoundOutput();
    virtual ~AndroidTempestSoundOutput();

    virtual void SetSoundChannelState(int channel, SoundChannelState state);

protected:
    virtual void ProcessTimeLapse(uint64_t clockCycles);

private:
    virtual void FillNextBuffer(WaveSoundSource *streamer);
    //void EnqueueBuffer(AudioQueueBufferRef buffer);

private:
    //static void AudioOutputCallback(void *inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer);

private:
    Cpp11WaveStreamer *waveStreamer = nullptr;
    //AudioQueueRef audioQueue;
    std::mutex bufferQueueMutex;
    //std::vector<AudioQueueBufferRef> bufferList;
    //std::vector<AudioQueueBufferRef> bufferQueue;
    uint64_t currentCPUTime = 0;
};


#endif //ANDROIDTEMPEST_ANDROIDTEMPESTSOUNDOUTPUT_H
