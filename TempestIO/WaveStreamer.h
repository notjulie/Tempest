
#ifndef WAVESTREAMER_H
#define WAVESTREAMER_H

#include "SoundGenerator.h"

#define WAVE_STREAM_EVENT_QUEUE_SIZE 1000
#define WAVE_STREAM_BUFFER_SAMPLE_COUNT 2000
#define WAVE_STREAM_INPUT_BUFFER_SAMPLE_COUNT (WAVE_STREAM_BUFFER_SAMPLE_COUNT*2)

class WaveStreamer
{
protected:
   enum WaveStreamEventType {
      WAVE_EVENT_VOLUME,
      WAVE_EVENT_FREQUENCY,
      WAVE_EVENT_WAVEFORM,
      WAVE_EVENT_TICK
   };

   struct WaveStreamEvent {
      WaveStreamEventType	eventType;
      uint8_t	channel;
      uint8_t	value;
   };


public:
   WaveStreamer(void);
   virtual ~WaveStreamer(void);

protected:
   void FillBuffer(int16_t *buffer, int sampleCount);
   bool ProcessNextEvent(void);
   virtual void  QueueEvent(const WaveStreamEvent &event);

private:
   void  ProcessTick(void);

private:
   SoundGenerator	soundGenerator;
   WaveStreamEvent	eventQueue[WAVE_STREAM_EVENT_QUEUE_SIZE];
   int	queueIn;
   int	queueOut;
   int   sampleCounter;
   int16_t inputBuffer[WAVE_STREAM_INPUT_BUFFER_SAMPLE_COUNT];
   int samplesInInputBuffer;
};

#endif
