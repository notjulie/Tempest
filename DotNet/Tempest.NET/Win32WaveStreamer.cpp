
#include "TempestIO.Headers.h"

#include "TempestCPU/TempestException.h"
#include "TempestIO/Cpp11WaveStreamer.h"

#include "Win32WaveStreamer.h"


#pragma comment(lib, "user32")
#pragma comment(lib, "WinMM")

Win32WaveStreamer::Win32WaveStreamer(void)
	:
		buffer1(WAVE_STREAM_BUFFER_SAMPLE_COUNT),
		buffer2(WAVE_STREAM_BUFFER_SAMPLE_COUNT)
{
	// clear
	waveOut = NULL;
	terminating = false;
	errorReported = false;

	WAVEFORMATEX waveFormat;
	memset(&waveFormat, 0, sizeof(waveFormat));
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nChannels = 1;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.nAvgBytesPerSec = 88200;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nBlockAlign = 2;
	waveFormat.cbSize = 0;

	MMRESULT openResult = waveOutOpen(
		&waveOut,
		WAVE_MAPPER,
		&waveFormat,
		(DWORD_PTR)&BufferCallback,
      (DWORD_PTR)this,
		CALLBACK_FUNCTION
		);
	if (openResult != MMSYSERR_NOERROR)
		return;

	// prepare the buffers
	buffer1.Prepare(waveOut);
	buffer2.Prepare(waveOut);

	// start the thread
   waveStreamer = new Cpp11WaveStreamer(WAVE_STREAM_BUFFER_SAMPLE_COUNT * 2, this);
}


Win32WaveStreamer::~Win32WaveStreamer(void)
{
	// mark ourself as terminating
	terminating = true;

	// stop playing
	waveOutReset(waveOut);

	// wait for both buffers to report that they aren't playing
	while (buffer1.IsPlaying() || buffer2.IsPlaying())
		Sleep(1);

	// stop the waveStreamer
   if (waveStreamer != nullptr)
   {
      delete waveStreamer;
      waveStreamer = nullptr;
   }

	// unprepare buffers
	buffer1.Unprepare(waveOut);
	buffer2.Unprepare(waveOut);

	// close the device
	if (waveOut != NULL)
	{
		waveOutClose(waveOut);
		waveOut = NULL;
	}
}

void Win32WaveStreamer::SetChannelState(int channel, SoundChannelState state)
{
   waveStreamer->SetChannelState(channel, state);
}

void Win32WaveStreamer::Delay(int clockCycles)
{
   waveStreamer->Delay(clockCycles);
}

void Win32WaveStreamer::FillNextBuffer(WaveSoundSource *source)
{
   // see if we have a buffer ready to fill
   Win32WaveBuffer *bufferToFill = nullptr;
   if (!terminating)
   {
      if (!buffer1.IsPlaying())
         bufferToFill = &buffer1;
      else if (!buffer2.IsPlaying())
         bufferToFill = &buffer2;
   }

   // fill the buffer to fill
   if (bufferToFill != nullptr)
   {
      // fill and play the buffer
      source->FillBuffer(bufferToFill->GetBuffer(), bufferToFill->GetSampleCount());
      bufferToFill->Play(waveOut);
   }
}

void CALLBACK Win32WaveStreamer::BufferCallback(
   HWAVEOUT  hwo,
   UINT      uMsg,
   DWORD_PTR dwInstance,
   DWORD_PTR dwParam1,
   DWORD_PTR dwParam2
   )
{
   switch (uMsg)
   {
   case WOM_DONE:
      ((Win32WaveBuffer *)((WAVEHDR *)dwParam1)->dwUser)->MarkDonePlaying();
      break;

   default:
      break;
   }
}


std::string Win32WaveStreamer::GetErrorString(void) const
{
	if (errorReported)
		return errorString;
	else
		return "OK";
}



