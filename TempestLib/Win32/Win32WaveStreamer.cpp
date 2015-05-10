
#include "stdafx.h"
#include "Win32.h"

#include "AbstractTempestWaveStream.h"
#include "TempestException.h"

#include "Win32WaveStreamer.h"

#pragma comment(lib, "user32")
#pragma comment(lib, "WinMM")

Win32WaveStreamer::Win32WaveStreamer(void)
{
	// clear
	waveOut = NULL;
	callbackThread = NULL;
	terminating = false;
	errorReported = false;
	source = NULL;

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
			ProcessFinishedBuffer((Win32WaveBuffer *)((WAVEHDR *)msg.lParam)->dwUser);
			break;

		default:
			break;
		}
	}
}


std::string Win32WaveStreamer::GetErrorString(void) const
{
	if (errorReported)
		return errorString;
	else
		return "OK";
}


void Win32WaveStreamer::ProcessFinishedBuffer(Win32WaveBuffer *waveBuffer)
{
	if (terminating)
	{
		waveBuffer->MarkDonePlaying();
	}
	else
	{
		try
		{
			FillBuffer(waveBuffer);
			waveBuffer->Play(waveOut);
		}
		catch (TempestException &x)
		{
			if (!errorReported)
			{
				errorString = x.what();
				errorReported = true;
			}
			waveBuffer->MarkDonePlaying();
		}
	}
}


void Win32WaveStreamer::FillBuffer(Win32WaveBuffer *buffer)
{
	int16_t *samples = buffer->GetBuffer();
	int count = buffer->GetSampleCount();

	// fill the buffer from the source
	source->ReadWaveData(samples, count);

	// the Pokey output is very low amplitude... beef it up to the level we like
	for (int i = 0; i < count; ++i)
		samples[i] = (int16_t)(samples[i] * 256);
}
