//
//  iTempestSoundIO.h
//  iTempest
//
//  Created by Randy Rasmussen on 11/18/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#ifndef iTempestSoundIO_h
#define iTempestSoundIO_h

#include <stdint.h>
#include <AudioToolbox/AudioQueue.h>
#include "AbstractTempestIO.h"
#include "Cpp11WaveStreamer.h"
#include "SoundChannelState.h"
#include "WaveSoundDriver.h"

class iTempestSoundIO : public AbstractTempestSoundIO, private WaveSoundDriver
{
public:
    iTempestSoundIO(void);
    
    void SetPlayer1ButtonState(bool state);
    
    virtual void SetSoundChannelState(int channel, SoundChannelState state) {};
    virtual void SetTime(uint64_t clockCycles) {};
    virtual uint8_t GetButtons(void) { return buttons; };
    virtual uint8_t GetEncoder(void) {return 0;};
    virtual void SetButtonLED(ButtonFlag button, bool value) {};
    
private:
    virtual void FillNextBuffer(WaveSoundSource *streamer);
    void EnqueueBuffer(AudioQueueBufferRef buffer);

private:
    static void AudioOutputCallback(void *inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer);
    
private:
    uint8_t buttons = 0;
    Cpp11WaveStreamer waveStreamer;
    AudioQueueRef audioQueue;
    std::mutex mutex;
    std::vector<AudioQueueBufferRef> bufferQueue;
};


#endif /* iTempestSoundIO_h */
