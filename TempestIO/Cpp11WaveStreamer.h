// ====================================================================
// Tempest emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is the WaveStreamer implementation for compilers that support
//    C++ 11.  This way I can standardize on the threading.
// ====================================================================

#ifndef CPP11WAVESTREAMER_H
#define CPP11WAVESTREAMER_H

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
};

#endif
