
#include "TempestDisco.h"

#include "Audio/DiscoWaveStreamer.h"
#include "SystemError.h"

#include "TempestDiscoAudioIO.h"

TempestDiscoAudioIO	IO;

void TempestDiscoAudioIO::SetSoundChannelFrequency(int channel, int frequency)
{
	DWS.SetChannelFrequency(channel, frequency);
}

void TempestDiscoAudioIO::SetSoundChannelVolume(int channel, int volume)
{
	DWS.SetChannelVolume(channel, volume);
}

void TempestDiscoAudioIO::SetSoundChannelWaveform(int channel, int waveform)
{
	DWS.SetChannelWaveform(channel, waveform);
}

void TempestDiscoAudioIO::Tick6KHz(void)
{
	DWS.Tick6KHz();
}

uint8_t TempestDiscoAudioIO::GetButtons(void)
{
	return 0;
}

uint8_t TempestDiscoAudioIO::GetEncoder(void)
{
	return 0;
}

