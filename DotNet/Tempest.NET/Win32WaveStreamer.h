// ====================================================================
// Tempest emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is a class that makes use of the generic Cpp11WaveStreamer
//    class to drive Win32 sound output.
//    
// ====================================================================


#ifndef WIN32WAVESTREAMER_H
#define WIN32WAVESTREAMER_H

#include "WaveSoundDriver.h"
#include "Win32WaveBuffer.h"

class Cpp11WaveStreamer;
class SoundChannelState;
class WaveSoundSource;

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

#define WAVE_STREAM_BUFFER_SAMPLE_COUNT 2000


class Win32WaveStreamer : public WaveSoundDriver
{
public:
	Win32WaveStreamer(void);
	~Win32WaveStreamer(void);

   void SetChannelState(int channel, SoundChannelState state);
   void Delay(int clockCycles);

protected:
   virtual void FillNextBuffer(WaveSoundSource *streamer);

private:
   static void CALLBACK BufferCallback(
      HWAVEOUT  hwo,
      UINT      uMsg,
      DWORD_PTR dwInstance,
      DWORD_PTR dwParam1,
      DWORD_PTR dwParam2
      );

private:
   Cpp11WaveStreamer *waveStreamer = nullptr;
	HWAVEOUT waveOut;

	bool terminating;

   Win32WaveBuffer buffer1;
	Win32WaveBuffer buffer2;
};


#pragma warning(pop)

#endif
