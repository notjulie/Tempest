
#ifndef WAVESTREAMER_H
#define WAVESTREAMER_H

#include "SoundGenerator.h"

#define WAVE_STREAM_EVENT_QUEUE_SIZE 1000


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
   WaveStreamer(int16_t *buffer, int _bufferSampleCount);
   virtual ~WaveStreamer(void);

   void SetChannelFrequency(int channel, int frequency);
   void SetChannelVolume(int channel, int volume);
   void SetChannelWaveform(int channel, int waveform);
   void Tick6KHz(void);

protected:
   void FillBuffer(int16_t *buffer, int sampleCount);
   bool ProcessNextEvent(void);
   virtual void  QueueEvent(const WaveStreamEvent &event);

private:
   void  ProcessTick(void);

private:
   int bufferSampleCount;
   int16_t *inputBuffer;
   int samplesInInputBuffer;

   SoundGenerator	soundGenerator;
   WaveStreamEvent	eventQueue[WAVE_STREAM_EVENT_QUEUE_SIZE];
   int	queueIn;
   int	queueOut;
   int   sampleCounter;
};

#endif
