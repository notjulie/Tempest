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
   virtual ~iTempestSoundIO(void);

   void MoveSpinner(int offset);
   bool GetPlayer1LEDState(void) const { return (buttonLEDs & ONE_PLAYER_BUTTON) != 0; }
   bool GetPlayer2LEDState(void) const { return (buttonLEDs & TWO_PLAYER_BUTTON) != 0; }
   void SetPlayer1ButtonState(bool state);
   void SetPlayer2ButtonState(bool state);
   void SetFireButtonState(bool state);
   void SetZapButtonState(bool state);

   virtual void SetSoundChannelState(int channel, SoundChannelState state);
   virtual void SetTime(uint64_t clockCycles);
   virtual uint8_t GetButtons(void) { return buttons; }
   virtual uint8_t GetEncoder(void);
   virtual void SetButtonLED(ButtonFlag button, bool value) { if (value) buttonLEDs |= button; else buttonLEDs &= ~button; }
    
private:
    virtual void FillNextBuffer(WaveSoundSource *streamer);
    void EnqueueBuffer(AudioQueueBufferRef buffer);

private:
    static void AudioOutputCallback(void *inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer);
    
private:
   uint8_t buttons = 0;
   uint8_t buttonLEDs = 0;
   uint8_t encoder = 0;
   Cpp11WaveStreamer *waveStreamer = nullptr;
   AudioQueueRef audioQueue;
   std::mutex bufferQueueMutex;
   std::vector<AudioQueueBufferRef> bufferList;
   std::vector<AudioQueueBufferRef> bufferQueue;
   uint64_t currentCPUTime = 0;
};


#endif /* iTempestSoundIO_h */
