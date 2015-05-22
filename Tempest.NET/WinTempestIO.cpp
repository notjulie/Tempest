
#include "Stdafx.h"
#include "Win32TempestIO.h"


Win32TempestIO::Win32TempestIO(void)
{
	terminating = false;

	// create the vector generator mutex
	vectorGeneratorMutex = CreateMutex(NULL, FALSE, NULL);

	// start our thread that polls the vector data
	vectorThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)VectorThreadEntry, this, 0, &vectorThreadID);
}

Win32TempestIO::~Win32TempestIO(void)
{
	// stop the thread
	terminating = true;
	WaitForSingleObject(vectorThread, INFINITE);
	CloseHandle(vectorThread);
	vectorThread = NULL;

	// close the mutex
	CloseHandle(vectorGeneratorMutex), vectorGeneratorMutex = NULL;
}

void Win32TempestIO::SetSoundChannelFrequency(int channel, int frequency)
{
	waveStreamer.SetChannelFrequency(channel, frequency);
}

void Win32TempestIO::SetSoundChannelVolume(int channel, int volume)
{
	waveStreamer.SetChannelVolume(channel, volume);
}

void Win32TempestIO::SetSoundChannelWaveform(int channel, int waveform)
{
	waveStreamer.SetChannelWaveform(channel, waveform);
}

void Win32TempestIO::Tick6KHz(void)
{
	waveStreamer.Tick6KHz();
}


void Win32TempestIO::VectorThread(void)
{
	while (!terminating)
	{
		Sleep(15);

		if (WaitForSingleObject(vectorGeneratorMutex, 100) != WAIT_OBJECT_0)
			continue;

		vectorInterpreter.ClearVectors();
		vectorInterpreter.Interpret();
		vectorInterpreter.GetAllVectors(currentVectors);

		ReleaseMutex(vectorGeneratorMutex);
	}
}


void Win32TempestIO::GetVectorList(std::vector<SimpleVector> &_vectorList)
{
	WaitForSingleObject(vectorGeneratorMutex, INFINITE);
	_vectorList = currentVectors;
	ReleaseMutex(vectorGeneratorMutex);
}
