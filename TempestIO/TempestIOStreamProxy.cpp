
#include "stdafx.h"

#include "../TempestCPU/TempestException.h"

#include "TempestIOStreamProxy.h"

TempestIOStreamProxy::TempestIOStreamProxy(AbstractTempestStream *stream)
{
	this->stream = stream;
}


void TempestIOStreamProxy::SetSoundChannelFrequency(int channel, int frequency)
{
	throw TempestException("TempestIOStreamProxy::SetSoundChannelFrequency: not implemented");
}

void TempestIOStreamProxy::SetSoundChannelVolume(int channel, int volume)
{
	throw TempestException("TempestIOStreamProxy::SetSoundChannelVolume: not implemented");
}

void TempestIOStreamProxy::SetSoundChannelWaveform(int channel, int waveform)
{
	throw TempestException("TempestIOStreamProxy::SetSoundChannelWaveform: not implemented");
}

void TempestIOStreamProxy::Tick6KHz(void)
{
	throw TempestException("TempestIOStreamProxy::Tick6KHz: not implemented");
}

void TempestIOStreamProxy::WriteVectorRAM(uint16_t address, uint8_t value)
{
	throw TempestException("TempestIOStreamProxy::WriteVectorRAM: not implemented");
}

bool TempestIOStreamProxy::IsVectorHalt(void)
{
	throw TempestException("TempestIOStreamProxy::IsVectorHalt: not implemented");
}

void TempestIOStreamProxy::VectorGo(void)
{
	throw TempestException("TempestIOStreamProxy::VectorGo: not implemented");
}

void TempestIOStreamProxy::VectorReset(void)
{
	throw TempestException("TempestIOStreamProxy::VectorReset: not implemented");
}

