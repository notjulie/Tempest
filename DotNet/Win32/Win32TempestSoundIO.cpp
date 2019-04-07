
#include "SoundChannelState.h"
#include "Win32WaveStreamer.h"
#include "Win32TempestSoundIO.h"


Win32TempestSoundIO::Win32TempestSoundIO(void)
{
   waveStreamer = new Win32WaveStreamer();
}

Win32TempestSoundIO::~Win32TempestSoundIO(void)
{
   delete waveStreamer;
   waveStreamer = nullptr;
}

void Win32TempestSoundIO::SetSoundChannelState(int channel, SoundChannelState state)
{
	waveStreamer->SetChannelState(channel, state);
}

void Win32TempestSoundIO::ProcessTimeLapse(uint64_t elapsedCycles)
{
	waveStreamer->Delay((int)(elapsedCycles));
}


