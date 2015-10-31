
#include "Stdafx.h"
#include "Win32TempestIO.h"


Win32TempestSoundIO::Win32TempestSoundIO(void)
{
   buttons = 0;
}

Win32TempestSoundIO::~Win32TempestSoundIO(void)
{
}

void Win32TempestSoundIO::SetSoundChannelFrequency(int channel, int frequency)
{
	waveStreamer.SetChannelFrequency(channel, frequency);
}

void Win32TempestSoundIO::SetSoundChannelVolume(int channel, int volume)
{
	waveStreamer.SetChannelVolume(channel, volume);
}

void Win32TempestSoundIO::SetSoundChannelWaveform(int channel, int waveform)
{
	waveStreamer.SetChannelWaveform(channel, waveform);
}

void Win32TempestSoundIO::Tick6KHz(void)
{
	waveStreamer.Tick6KHz();
}


void Win32TempestSoundIO::Fire(bool state)
{
   if (state)
      buttons |= FIRE_BUTTON;
   else
      buttons &= ~FIRE_BUTTON;
}


void Win32TempestSoundIO::OnePlayer(bool state)
{
   if (state)
      buttons |= ONE_PLAYER_BUTTON;
   else
      buttons &= ~ONE_PLAYER_BUTTON;
}


void Win32TempestSoundIO::Zap(bool state)
{
   if (state)
      buttons |= ZAPPER_BUTTON;
   else
      buttons &= ~ZAPPER_BUTTON;
}




Win32TempestVectorIO::Win32TempestVectorIO(void)
{
   terminating = false;

   // create the vector generator mutex
   vectorGeneratorMutex = CreateMutex(NULL, FALSE, NULL);

   // start our thread that polls the vector data
   vectorThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)VectorThreadEntry, this, 0, &vectorThreadID);
}

Win32TempestVectorIO::~Win32TempestVectorIO(void)
{
   // stop the thread
   terminating = true;
   WaitForSingleObject(vectorThread, INFINITE);
   CloseHandle(vectorThread);
   vectorThread = NULL;

   // close the mutex
   CloseHandle(vectorGeneratorMutex), vectorGeneratorMutex = NULL;
}

void Win32TempestVectorIO::VectorThread(void)
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


void Win32TempestVectorIO::GetVectorList(std::vector<SimpleVector> &_vectorList)
{
   WaitForSingleObject(vectorGeneratorMutex, INFINITE);
   _vectorList = currentVectors;
   ReleaseMutex(vectorGeneratorMutex);
}

