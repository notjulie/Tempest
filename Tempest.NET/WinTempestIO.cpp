
#include "Stdafx.h"
#include "Win32TempestIO.h"


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
