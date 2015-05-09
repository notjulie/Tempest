
#include "stdafx.h"
#include "Win32.h"

#include "Win32WaveStreamer.h"

#pragma comment(lib, "user32")
#pragma comment(lib, "WinMM")

Win32WaveStreamer::Win32WaveStreamer(void)
{
	// clear
	waveOut = NULL;
	callbackThread = NULL;

	// create our callback thread
	callbackThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CallbackThreadEntry, this, 0, &callbackThreadID);
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

	// test play
	buffer1.Play(waveOut);
	buffer2.Play(waveOut);
}


Win32WaveStreamer::~Win32WaveStreamer(void)
{
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
}


void Win32WaveStreamer::CallbackThread(void)
{
	for (;;)
	{
		MSG msg;
		if (!GetMessage(&msg, NULL, 0, 0))
			break;

		switch (msg.message)
		{
		case MM_WOM_DONE:
			{
				WAVEHDR *waveHdr = (WAVEHDR *)msg.lParam;
				Win32WaveBuffer *waveBuffer = (Win32WaveBuffer *)waveHdr->dwUser;
				waveBuffer->Play(waveOut);
			}
			break;

		default:
			break;
		}
	}
}
