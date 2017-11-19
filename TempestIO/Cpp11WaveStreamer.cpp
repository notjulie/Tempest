
#include "stdafx.h"
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
      thread->join();
      delete thread;
      thread = nullptr;
   }
}


void Cpp11WaveStreamer::ThreadEntry(void)
{
   std::mutex mutex;
   std::unique_lock<std::mutex> lock(mutex);

   for (;;)
   {
      // Wait for an event... if there are no events this prevents us from
      // checking for a finished buffer, but the events come extremely often so it's
      // not a concern.  Nonetheless we still limit it.
      queueEvent.wait_for(lock, std::chrono::milliseconds(50));

      // fill the next output buffer if we have one to fill
      target->FillNextBuffer(this);

      // if we still have events to process, process them
      while (ProcessNextEvent())
         continue;
   }
}

/// <summary>
/// Sets an event to let our thread know that there is incoming
/// sound data to be processed.
/// </summary>
void Cpp11WaveStreamer::OnWaveStreamEventQueued(void)
{
   queueEvent.notify_all();
}
