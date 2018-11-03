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
//    For systems supporting C++11, the Cpp11WaveStreamer implements this,
//    leaving the derived class only to deal with transferring sound
//    data to the sound hardware (by calling FillBuffer).
//
//    Note that the generic implementation needs to avoid C++11 features,
//    as its original target was (and is) an embedded processor that
//    doesn't support things such as std::thread.  So keep this simple.
// ====================================================================

#include "TempestIO.Headers.h"
#include <string.h>

#include "WaveStreamer.h"


/// <summary>
/// Constructor
/// </summary>
/// <param name="buffer">The working buffer into which the sound is staged
/// while waiting for FillBuffer to be called.</param>
/// <param name="bufferSampleCount">Number of items in the working buffer;
/// this should of course as small as it can be without being too small...
/// if too large it will increase the delay between the action on the screen
/// and the sound, if too small sounds may end up distorted.</param>
WaveStreamer::WaveStreamer(int16_t *buffer, int _bufferSampleCount)
{
   // save parameters
   inputBuffer = buffer;
   bufferSampleCount = _bufferSampleCount;

   // clear
   queueIn = 0;
   queueOut = 0;
   clockCycleCounter = 0;
   samplesInInputBuffer = 0;
}

/// <summary>
/// Default constructor; object will not be fully constructed until after calling
/// SetBuffer().
/// </summary>
WaveStreamer::WaveStreamer(void)
{
   // clear
   inputBuffer = NULL;
   bufferSampleCount = 0;
   queueIn = 0;
   queueOut = 0;
   clockCycleCounter = 0;
   samplesInInputBuffer = 0;
}

/// <summary>
/// Sets the buffer to use for sound generation.
/// </summary>
/// <param name="buffer">The working buffer into which the sound is staged
/// while waiting for FillBuffer to be called.</param>
/// <param name="bufferSampleCount">Number of items in the working buffer;
/// this should of course as small as it can be without being too small...
/// if too large it will increase the delay between the action on the screen
/// and the sound, if too small sounds may end up distorted.</param>
void WaveStreamer::SetBuffer(int16_t *buffer, int _bufferSampleCount)
{
   inputBuffer = buffer;
   bufferSampleCount = _bufferSampleCount;
}


/// <summary>
/// Destructor
/// </summary>
WaveStreamer::~WaveStreamer(void)
{
}

/// <summary>
/// Enqueues a change to the sound channel state; this does no sound processing,
/// it is intended to be called by the 6502 emulator and is therefore handled
/// asynchronously so that some other thread can generate the sound data.
/// </summary>
/// <param name="channel">The target sound channel</param>
/// <param name="state">The new state of the channel</param>
void WaveStreamer::SetChannelState(int channel, SoundChannelState state)
{
   // don't bother queueing no-op commands
   if (previousState[channel] == state)
      return;
   previousState[channel] = state;

   WaveStreamEvent	event;
   event.eventType = WAVE_EVENT_CHANNEL_STATE;
   event.channel = channel;
   event.channelState = state;
   QueueEvent(event);
}


/// <summary>
/// Enqueues an event indicating that a given number of 6502 cycles have elapsed.
/// This is required between calls to SetChannelState to indicate the delay between
/// the sound events so that the delays can be reproduced in the sound output.  As
/// with SetChannelState, this just enqueues an event so that the caller is not
/// burderned with actual sound generation.
/// </summary>
/// <param name="channel">The number of elapsed clock cycles</param>
void WaveStreamer::Delay(int clockCycles)
{
   WaveStreamEvent	event;
   event.eventType = WAVE_EVENT_DELAY;
   event.delay = clockCycles;
   QueueEvent(event);
}


/// <summary>
/// Returns a value indicating whether any of the sound channels currently has a
/// non-zero volume.
/// </summary>
bool WaveStreamer::HaveSoundOutput(void)
{
   return soundGenerator.HaveSoundOutput();
}


/// <summary>
/// Needs to be called by the sound output thread to process incoming
/// data and generate the sound data before the sound output thread calls FillBuffer().
/// </summary>
/// <return>a flag indicating whether there was data to process</return>
bool WaveStreamer::ProcessNextEvent(void)
{
   // never mind if there are none in the queue
   if (queueIn == queueOut)
      return false;

   // grab from the queue
   WaveStreamEvent event = eventQueue[queueOut];
   if (queueOut == WAVE_STREAM_EVENT_QUEUE_SIZE - 1)
      queueOut = 0;
   else
      queueOut++;

   // process it
   switch (event.eventType)
   {
   case WAVE_EVENT_CHANNEL_STATE:
      soundGenerator.SetChannelState(event.channel, event.channelState);
      break;

   case WAVE_EVENT_DELAY:
      ProcessDelay(event.delay);
      break;
   }

   return true;
}


/// <summary>
/// Needs to be called by the sound output thread to transfer sound data
/// to the output buffer.  In a double-buffering sound output system, this
/// should be called when a new buffer becomes available.
/// </summary>
void WaveStreamer::FillBuffer(int16_t *buffer, int sampleCount)
{
   // if we don't have enough data in our input buffer to fill an output
   // buffer then we need to process messages
   while (samplesInInputBuffer < sampleCount)
      if (!ProcessNextEvent())
         break;

   // if we don't have enough data fake the passage of time until we do
   while (samplesInInputBuffer < sampleCount)
      ProcessDelay(250);

   // copy the data... the Pokey output is very low amplitude... beef it up to the level we like
   for (int i = 0; i < sampleCount; ++i)
      buffer[i] = (int16_t)(inputBuffer[(unsigned)i] * 256);

   // remove the copied data from the input buffer
   samplesInInputBuffer -= sampleCount;
   memcpy(&inputBuffer[0], &inputBuffer[sampleCount], 2 * samplesInInputBuffer);
}

/// <summary>
/// This can be overridden to take action when a sound event is enqueued; for
/// example, Cpp11WaveStreamer uses it to set an event that signals the sound
/// output thread to begin calling ProcessNextEvent().
/// </summary>
void WaveStreamer::OnWaveStreamEventQueued(void)
{
}

/// <summary>
/// Low level sound processing... generates sound data in the buffer corresponding
/// to the amount of elapsed time indicated by the given number of clock cycles.
/// </summary>
/// <param name="clockCycles">the number of elapsed clock cycles</param>
void WaveStreamer::ProcessDelay(int clockCycles)
{
   // update our clock cycle count
   clockCycleCounter += clockCycles;

   // figure out how many samples to add for this tick
   int samplesToAdd = clockCycleCounter * 44100 / 1500000;
   clockCycleCounter -= samplesToAdd * 1500000 / 44100;

   // make sure we have room
   if (samplesInInputBuffer + samplesToAdd > bufferSampleCount)
      samplesToAdd = bufferSampleCount - samplesInInputBuffer;

   // generate some samples
   soundGenerator.ReadWaveData(&inputBuffer[samplesInInputBuffer], samplesToAdd);
   samplesInInputBuffer += samplesToAdd;
}


/// <summary>
/// Puts an event into the queue
/// </summary>
/// <param name="event">the event to enqueue</param>
void WaveStreamer::QueueEvent(const WaveStreamEvent &event)
{
   // We are just doing this in a good old fashioned cooperative
   // circular buffer... it is thread safe as long as there is only
   // one reader and one writer.  We are the writer.

   // figure out what the index will be after we add the event
   int nextIndex = queueIn + 1;
   if (nextIndex >= WAVE_STREAM_EVENT_QUEUE_SIZE)
      nextIndex = 0;

   // if it would result in an overflow just drop this event
   if (nextIndex == queueOut)
      return;

   // enqueue the event
   eventQueue[queueIn] = event;
   queueIn = nextIndex;

   // notify
   OnWaveStreamEventQueued();
}


