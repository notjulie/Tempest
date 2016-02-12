
#ifndef WAVESTREAMER_H
#define WAVESTREAMER_H

#include "SoundGenerator.h"

#define WAVE_STREAM_EVENT_QUEUE_SIZE 1000


class WaveStreamer
{
protected:
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

public:
   WaveStreamer(int16_t *buffer, int _bufferSampleCount);
   virtual ~WaveStreamer(void);

   void SetChannelState(int channel, SoundChannelState state);
   void Delay(int clockCycles);

   bool HaveSoundOutput(void);

protected:
   void FillBuffer(int16_t *buffer, int sampleCount);
   bool ProcessNextEvent(void);
   virtual void  QueueEvent(const WaveStreamEvent &event);

private:
   void  ProcessDelay(int clockCycles);

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
