
#ifndef CPP11WAVESTREAMER_H
#define CPP11WAVESTREAMER_H

#include <condition_variable>
#include <thread>
#include <vector>
#include "WaveStreamer.h"

class WaveSoundDriver;

class Cpp11WaveStreamer final : public WaveStreamer
{
public:
   Cpp11WaveStreamer(uint32_t bufferSampleCount, WaveSoundDriver *_target);
   virtual ~Cpp11WaveStreamer(void);

protected:
   virtual void  OnWaveStreamEventQueued(void);

private:
   void ThreadEntry(void);

private:
   WaveSoundDriver *target;
   std::vector<int16_t> buffer;
   std::thread *thread = nullptr;
   std::condition_variable queueEvent;
};

#endif
