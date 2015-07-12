
#include "TempestDisco.h"

#include "Audio/DiscoWaveStreamer.h"
#include "SystemError.h"

#include "TempestDiscoIO.h"

TempestDiscoIO	IO;

void TempestDiscoIO::SetSoundChannelFrequency(int channel, int frequency)
{
	DWS.SetChannelFrequency(channel, frequency);
}

void TempestDiscoIO::SetSoundChannelVolume(int channel, int volume)
{
	DWS.SetChannelVolume(channel, volume);
}

void TempestDiscoIO::SetSoundChannelWaveform(int channel, int waveform)
{
	DWS.SetChannelWaveform(channel, waveform);
}

void TempestDiscoIO::Tick6KHz(void)
{
	DWS.Tick6KHz();
}

void TempestDiscoIO::WriteVectorRAM(uint16_t address, uint8_t value)
{
}

bool TempestDiscoIO::IsVectorHalt(void)
{
	return true;
}

void TempestDiscoIO::VectorGo(void)
{
}

void TempestDiscoIO::VectorReset(void)
{
}

