// =========================================================================
//    TempestIOStreamProxy.cpp
//    Author: Randy Rasmussen
//    Copyright: none, use at your own risk and to your heart's content
//
//		This is an implementation of TempestIO that connects to the actual
//    IO device via a stream.  My plan is to use a USB-to-serial connection,
//    but for testing it's most convenient to leave this as generic as
//    possible.
// =========================================================================

#include "stdafx.h"

#include "../TempestCPU/TempestException.h"
#include "AbstractTempestStream.h"
#include "TempestStreamProtocol.h"

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
	// send just the opcode with no data
	stream->Write(OP_6KHZ_TICK << 5);
}

void TempestIOStreamProxy::WriteVectorRAM(uint16_t address, uint8_t value)
{
	// the address is 12 bits, so the 4 high bits go in the op byte
	stream->Write((OP_WRITE_VECTOR_RAM << 5) | (address>>8));

	// then the rest of the address and the value
	stream->Write((uint8_t)address);
	stream->Write(value);
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
	// send just the opcode with no data
	stream->Write(OP_VECTOR_RESET << 5);
}

