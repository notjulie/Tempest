
#include "stdafx.h"

#include "TempestCPU/TempestException.h"

#include "Win32WaveStreamer.h"


#pragma comment(lib, "user32")
#pragma comment(lib, "WinMM")

Win32WaveStreamer::Win32WaveStreamer(void)
	:
      WaveStreamer(waveStreamBuffer, WAVE_STREAM_BUFFER_SAMPLE_COUNT * 2),
		buffer1(WAVE_STREAM_BUFFER_SAMPLE_COUNT),
		buffer2(WAVE_STREAM_BUFFER_SAMPLE_COUNT)
{
	// clear
	waveOut = NULL;
	callbackThread = NULL;
	terminating = false;
	errorReported = false;

	// create our event
	queueEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// create our callback thread
	callbackThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CallbackThreadEntry, this, CREATE_SUSPENDED, &callbackThreadID);
	SetThreadPriority(callbackThread, THREAD_PRIORITY_TIME_CRITICAL);

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
		(DWORD_PTR)callbackThreadID,
		NULL,
		CALLBACK_THREAD
		);
	if (openResult != MMSYSERR_NOERROR)
		return;

	// prepare the buffers
	buffer1.Prepare(waveOut);
	buffer2.Prepare(waveOut);

	// start the thread
	ResumeThread(callbackThread);
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

	// stop the thread
	if (callbackThread != NULL)
	{
		PostThreadMessage(callbackThreadID, WM_QUIT, 0, 0);
		WaitForSingleObject(callbackThread, INFINITE);
		CloseHandle(callbackThread);
		callbackThread = NULL;
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

	// clean up
	if (queueEvent != NULL)
		CloseHandle(queueEvent), queueEvent = NULL;
}


void Win32WaveStreamer::CallbackThread(void)
{
	for (;;)
	{
		// wait for an event... if there are no events this prevents us from
		// checking for a finished buffer, but the events come extremely often so it's
		// not a concern
		WaitForSingleObject(queueEvent, 50);

		// process messages from the Windows message queue
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case MM_WOM_DONE:
				((Win32WaveBuffer *)((WAVEHDR *)msg.lParam)->dwUser)->MarkDonePlaying();
				break;

			case WM_QUIT:
				return;

			default:
				break;
			}
		}

		// see if we have a buffer ready to fill
		Win32WaveBuffer *bufferToFill = NULL;
		if (!terminating)
		{
			if (!buffer1.IsPlaying())
				bufferToFill = &buffer1;
			else if (!buffer2.IsPlaying())
				bufferToFill = &buffer2;
		}

      // fill the buffer to fill
      if (bufferToFill != NULL)
      {
         // fill and play the buffer
         FillBuffer(bufferToFill->GetBuffer(), bufferToFill->GetSampleCount());
         bufferToFill->Play(waveOut);
      }

		// if we still have events to process, process them
		while (ProcessNextEvent())
			continue;

      // this doesn't need to be synchronized... if we don't trigger on the
      // next event the one after it will follow shortly
      ResetEvent(queueEvent);
	}
}




std::string Win32WaveStreamer::GetErrorString(void) const
{
	if (errorReported)
		return errorString;
	else
		return "OK";
}




void Win32WaveStreamer::QueueEvent(const WaveStreamEvent &event)
{
   WaveStreamer::QueueEvent(event);
   SetEvent(queueEvent);
}
