
#include "Tempest.NET.h"
#include "SoundChannelState.h"
#include "Win32TempestIO.h"


Win32TempestSoundIO::Win32TempestSoundIO(void)
{
}

Win32TempestSoundIO::~Win32TempestSoundIO(void)
{
}

void Win32TempestSoundIO::SetSoundChannelState(int channel, SoundChannelState state)
{
	waveStreamer.SetChannelState(channel, state);
}

void Win32TempestSoundIO::ProcessTimeLapse(uint64_t elapsedCycles)
{
	waveStreamer.Delay((int)(elapsedCycles));
}


