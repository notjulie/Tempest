
#include "stdafx.h"
#include "Win32.h"

#include "AbstractTempestWaveStream.h"
#include "TempestException.h"

#include "Win32WaveStreamer.h"

static const int BUFFER_SAMPLE_COUNT = 2000;

#pragma comment(lib, "user32")
#pragma comment(lib, "WinMM")

Win32WaveStreamer::Win32WaveStreamer(void)
	:
		buffer1(BUFFER_SAMPLE_COUNT),
		buffer2(BUFFER_SAMPLE_COUNT)
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

		try
		{
			switch (msg.message)
			{
			case MM_WOM_DONE:
				ProcessFinishedBuffer((Win32WaveBuffer *)((WAVEHDR *)msg.lParam)->dwUser);
				break;

			case WM_USER:
				ProcessUpdate((int)msg.lParam);
				break;

			default:
				break;
			}
		}
		catch (TempestException &x)
		{
			if (!errorReported)
				errorString = x.what();
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
	// fill up our input buffer if it's not
	if (inputBuffer.size() < BUFFER_SAMPLE_COUNT)
	{
		unsigned offset = (unsigned)inputBuffer.size();
		inputBuffer.resize(BUFFER_SAMPLE_COUNT);
		source->ReadWaveData(&inputBuffer[offset], (int)(BUFFER_SAMPLE_COUNT - offset));
	}

	int16_t *samples = buffer->GetBuffer();
	int count = buffer->GetSampleCount();

	// fill the buffer from the source
	source->ReadWaveData(samples, count);

	// copy the data... the Pokey output is very low amplitude... beef it up to the level we like
	for (int i = 0; i < count; ++i)
		samples[i] = (int16_t)(inputBuffer[(unsigned)i] * 256);

	// empty the input buffer
	inputBuffer.resize(0);
}


void Win32WaveStreamer::Update(int msElapsed)
{
	// this needs to happen on the callback thread
	PostThreadMessage(callbackThreadID, WM_USER, 0, msElapsed);
}


void Win32WaveStreamer::ProcessUpdate(int msElapsed)
{
	// figure out how many samples we should get for the amount of
	// time elapsed
	int samplesToRead = 44100 * msElapsed / 1000;
	
	// never get more than what would fill up our buffer
	if (samplesToRead > (int)(BUFFER_SAMPLE_COUNT - inputBuffer.size()))
		samplesToRead = (int)(BUFFER_SAMPLE_COUNT - inputBuffer.size());
	if (samplesToRead == 0)
		return;

	int offset = (int)inputBuffer.size();
	inputBuffer.resize((unsigned)(offset + samplesToRead));
	source->ReadWaveData(&inputBuffer[(unsigned)offset], samplesToRead);
}
