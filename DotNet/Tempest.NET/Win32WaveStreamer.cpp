
#include "stdafx.h"

#include "TempestCPU/TempestException.h"

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
	callbackThread = NULL;
	terminating = false;
	errorReported = false;
	queueIn = 0;
	queueOut = 0;
	samplesInInputBuffer = 0;
	sampleCounter = 0;

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

		// if we don't have enough data in our input buffer to fill an output
		// buffer then we need to process messages
		while (samplesInInputBuffer < WAVE_STREAM_BUFFER_SAMPLE_COUNT)
			if (!ProcessEvent())
				break;

		// if we have a buffer to fill we need to fill it
		if (bufferToFill != NULL)
		{
			// if we don't have enough data fake the passage of time until we do
			while (samplesInInputBuffer < WAVE_STREAM_BUFFER_SAMPLE_COUNT)
				ProcessTick();

			// fill the buffer
			int16_t *samples = bufferToFill->GetBuffer();
			int count = bufferToFill->GetSampleCount();

			// copy the data... the Pokey output is very low amplitude... beef it up to the level we like
			for (int i = 0; i < count; ++i)
				samples[i] = (int16_t)(inputBuffer[(unsigned)i] * 256);

			// remove the copied data from the input buffer
			samplesInInputBuffer -= count;
			memcpy(&inputBuffer[0], &inputBuffer[count], 2 * samplesInInputBuffer);

			// write out the buffer
			bufferToFill->Play(waveOut);
		}

		// if we still have events to process, process them
		while (ProcessEvent())
			continue;
	}
}


bool Win32WaveStreamer::ProcessEvent(void)
{
	// never mind if there are none in the queue
	if (queueIn == queueOut)
	{
		ResetEvent(queueEvent);
		return false;
	}

	// grab from the queue
	WaveStreamEvent event = eventQueue[queueOut];
	if (queueOut == WAVE_STREAM_EVENT_QUEUE_SIZE - 1)
		queueOut = 0;
	else
		queueOut++;

	// process it
	switch (event.eventType)
	{
	case WAVE_EVENT_VOLUME:
		soundGenerator.SetChannelVolume(event.channel, event.value);
		break;

	case WAVE_EVENT_FREQUENCY:
		soundGenerator.SetChannelFrequency(event.channel, event.value);
		break;

	case WAVE_EVENT_WAVEFORM:
		soundGenerator.SetChannelWaveform(event.channel, event.value);
		break;

	case WAVE_EVENT_TICK:
		ProcessTick();
		break;
	}

	return true;
}


void Win32WaveStreamer::ProcessTick(void)
{
	// figure out how many samples to add for this tick
	sampleCounter += 44100.0F / 6000.0F;
	int samplesToAdd = (int)sampleCounter;
	sampleCounter -= samplesToAdd;

	// make sure we have room
	if (samplesInInputBuffer + samplesToAdd > WAVE_STREAM_INPUT_BUFFER_SAMPLE_COUNT)
		samplesToAdd = WAVE_STREAM_INPUT_BUFFER_SAMPLE_COUNT - samplesInInputBuffer;

	// generate some samples
	soundGenerator.ReadWaveData(&inputBuffer[samplesInInputBuffer], samplesToAdd);
	samplesInInputBuffer += samplesToAdd;
}



std::string Win32WaveStreamer::GetErrorString(void) const
{
	if (errorReported)
		return errorString;
	else
		return "OK";
}




void Win32WaveStreamer::SetChannelFrequency(int channel, int frequency)
{
	WaveStreamEvent	event;
	event.eventType = WAVE_EVENT_FREQUENCY;
	event.channel = channel;
	event.value = frequency;
	QueueEvent(event);
}

void Win32WaveStreamer::SetChannelVolume(int channel, int volume)
{
	WaveStreamEvent	event;
	event.eventType = WAVE_EVENT_VOLUME;
	event.channel = channel;
	event.value = volume;
	QueueEvent(event);
}

void Win32WaveStreamer::SetChannelWaveform(int channel, int waveform)
{
	WaveStreamEvent	event;
	event.eventType = WAVE_EVENT_WAVEFORM;
	event.channel = channel;
	event.value = waveform;
	QueueEvent(event);
}

void Win32WaveStreamer::Tick6KHz(void)
{
	WaveStreamEvent	event;
	event.eventType = WAVE_EVENT_TICK;
	QueueEvent(event);
}


void Win32WaveStreamer::QueueEvent(const WaveStreamEvent &event)
{
	// figure out what the index will be after we add the event
	int nextIndex = queueIn + 1;
	if (nextIndex >= WAVE_STREAM_EVENT_QUEUE_SIZE)
		nextIndex = 0;

	// if it would result in a wraparound just drop this event
	if (nextIndex == queueOut)
		return;

	// enqueue and set the event
	eventQueue[queueIn] = event;
	queueIn = nextIndex;
	SetEvent(queueEvent);
}
