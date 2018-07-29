//
//  iTempestSoundIO.h
//  iTempest
//
//  Created by Randy Rasmussen on 11/18/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#ifndef iTempestSoundIO_h
#define iTempestSoundIO_h

#include <mutex>
#include <stdint.h>
#include <AudioToolbox/AudioQueue.h>
#include "AbstractSoundGenerator.h"
#include "Cpp11WaveStreamer.h"
#include "SoundChannelState.h"
#include "WaveSoundDriver.h"

class iTempestSoundOutput : public AbstractSoundGenerator, private WaveSoundDriver
{
public:
   iTempestSoundOutput(void);
   virtual ~iTempestSoundOutput(void);

   virtual void SetSoundChannelState(int channel, SoundChannelState state);
   
protected:
   virtual void ProcessTimeLapse(uint64_t clockCycles);
   
private:
    virtual void FillNextBuffer(WaveSoundSource *streamer);
    void EnqueueBuffer(AudioQueueBufferRef buffer);

private:
    static void AudioOutputCallback(void *inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer);
    
private:
   Cpp11WaveStreamer *waveStreamer = nullptr;
   AudioQueueRef audioQueue;
   std::mutex bufferQueueMutex;
   std::vector<AudioQueueBufferRef> bufferList;
   std::vector<AudioQueueBufferRef> bufferQueue;
   uint64_t currentCPUTime = 0;
};


#endif /* iTempestSoundIO_h */
