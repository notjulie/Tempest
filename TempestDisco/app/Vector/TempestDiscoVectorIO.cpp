
#include "TempestDisco.h"

#include "Audio/DiscoWaveStreamer.h"
#include "SystemError.h"

#include "TempestDiscoVectorIO.h"

TempestDiscoVectorIO	IO;

void TempestDiscoVectorIO::SetSoundChannelFrequency(int channel, int frequency)
{
}

void TempestDiscoVectorIO::SetSoundChannelVolume(int channel, int volume)
{
}

void TempestDiscoVectorIO::SetSoundChannelWaveform(int channel, int waveform)
{
}

void TempestDiscoVectorIO::Tick6KHz(void)
{
}

void TempestDiscoVectorIO::WriteVectorRAM(uint16_t address, uint8_t value)
{
}

bool TempestDiscoVectorIO::IsVectorHalt(void)
{
	return true;
}

void TempestDiscoVectorIO::VectorGo(void)
{
}

void TempestDiscoVectorIO::VectorReset(void)
{
}

