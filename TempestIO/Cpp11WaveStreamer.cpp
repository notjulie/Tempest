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

#include "TempestIO.Headers.h"
#include "WaveSoundDriver.h"

#include "Cpp11WaveStreamer.h"


Cpp11WaveStreamer::Cpp11WaveStreamer(uint32_t bufferSampleCount, WaveSoundDriver *_target)
{
   // save parameters
   target = _target;

   // set our buffer
   buffer.resize(bufferSampleCount);
   SetBuffer(&buffer[0], bufferSampleCount);

   // start our thread
   thread = new std::thread([this](){ ThreadEntry(); });
}

Cpp11WaveStreamer::~Cpp11WaveStreamer(void)
{
   if (thread != nullptr)
   {
      // signal termination
      terminated = true;
      
      // wait for the thread to shut itself down
      thread->join();
      delete thread;
      thread = nullptr;
   }
}


void Cpp11WaveStreamer::ThreadEntry(void)
{
   while (!terminated)
   {
      // Formerly I did this with a condition_variable/mutex.  Since we get thousands of
      // queue entries per second, this gets to be expensive, and I found that even calling
      // sleep_for 1000 times a second was expensive on iOS.  It seems better just to let
      // queue entries accumulate a little bit and then handle a burst of them.
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      
      // fill the next output buffer if we have one to fill
      target->FillNextBuffer(this);

      // if we still have events to process, process them
      while (ProcessNextEvent())
         continue;
   }
}

