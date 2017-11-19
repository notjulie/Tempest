// ====================================================================
// Tempest emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    Tempest generic sound processor; receives commands from TempestBus
//    (its memory-mapped IO calls our methods eventually).  It requires
//    a derived implementation to call its ProcessNextEvent method.
//    For systems supporting C++11, the C11WaveStreamer implements this,
//    leaving the derived class only to deal with transferring sound
//    data to the sound hardware (by calling FillBuffer).
//
//    Note that the generic implementation needs to avoid C++11 features,
//    as its original target was (and is) an embedded processor that
//    doesn't support things such as std::thread.  So keep this simple.
// ====================================================================


#ifndef WAVESTREAMER_H
#define WAVESTREAMER_H

#include "SoundGenerator.h"

#define WAVE_STREAM_EVENT_QUEUE_SIZE 1000

class WaveSoundSource
{
public:
   virtual void FillBuffer(int16_t *buffer, int sampleCount) = 0;
};

class WaveStreamer : protected WaveSoundSource
{
public:
   WaveStreamer(void);
   WaveStreamer(int16_t *buffer, int _bufferSampleCount);
   void SetBuffer(int16_t *buffer, int _bufferSampleCount);
   virtual ~WaveStreamer(void);

   void SetChannelState(int channel, SoundChannelState state);
   void Delay(int clockCycles);

   bool HaveSoundOutput(void);

protected:
   virtual void FillBuffer(int16_t *buffer, int sampleCount);
   bool ProcessNextEvent(void);
   virtual void OnWaveStreamEventQueued(void);

private:
   enum WaveStreamEventType {
      WAVE_EVENT_CHANNEL_STATE,
      WAVE_EVENT_DELAY
   };

   struct WaveStreamEvent {
      WaveStreamEventType	eventType;
      uint8_t	channel;
      SoundChannelState channelState;
      int  delay;
   };

private:
   void  ProcessDelay(int clockCycles);
   void  QueueEvent(const WaveStreamEvent &event);

private:
   int bufferSampleCount;
   int16_t *inputBuffer;
   int samplesInInputBuffer;

   SoundChannelState previousState[8];

   SoundGenerator	soundGenerator;
   WaveStreamEvent	eventQueue[WAVE_STREAM_EVENT_QUEUE_SIZE];
   int	queueIn;
   int	queueOut;
   int clockCycleCounter;
};

#endif
