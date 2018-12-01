// ====================================================================
// DiscoWaveStreamer.cpp
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    Contains the implementation of class DiscoWaveStreamer.  The parent
//    class, WaveStreamer, is a simple sound generator based originally
//    on simulating the Atari POKEY chip's sound generation.  Our job,
//    as the child class is to wait for the hardware to be ready to
//    output some sound and then stream some sound data from the base
//    class to the sound hardware.
// ====================================================================

#include "TempestDisco.h"
#include "AudioDriver.h"
#include "DiscoWaveStreamer.h"

/// <summary>
/// Our global instance
/// </summary>
DiscoWaveStreamer DWS;


/// <summary>
/// Initializes a new instance of class DiscoWaveStreamer
/// </summary>
DiscoWaveStreamer::DiscoWaveStreamer(void)
	:
		WaveStreamer(buffer, TEMPEST_DISCO_SOUND_BUFFER_SAMPLE_COUNT)
{
}


/// <summary>
/// Must be called periodically (frequently, at least 200Hz) to check to
/// see if a sound buffer is ready to be filled with sound from the stream,
/// and fills it, if so.
/// </summary>
void DiscoWaveStreamer::Service(void)
{
   // see if we have a buffer ready to fill
   int16_t *bufferToFill;
   int frameCount;
   if (AudioDriverPopEmptyBuffer(&bufferToFill, &frameCount))
   {
   	// we need to convert mono to stereo... start by filling the
   	// buffer mono
      FillBuffer(bufferToFill, frameCount);

      // then make it stereo
      for (int i=frameCount-1; i>=0; --i)
      {
      	bufferToFill[i*2 + 1] = bufferToFill[i];
      	bufferToFill[i * 2] = bufferToFill[i];
      }
   }

	// if we still have events to process, process them
	while (ProcessNextEvent())
		continue;
}

