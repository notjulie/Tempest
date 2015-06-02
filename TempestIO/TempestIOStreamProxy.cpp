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


TempestIOStreamProxy::TempestIOStreamProxy(AbstractTempestOutStream *stream)
{
   // copy parameters
	this->stream = stream;

   // clear
   isVectorHalt = true;
}


void TempestIOStreamProxy::SetSoundChannelFrequency(int channel, int frequency)
{
   // pack the op and the channel into the first byte
   stream->Write((OP_SOUND_FREQUENCY << 5) | channel);

   // waveform in the second byte
   stream->Write(frequency);
}

void TempestIOStreamProxy::SetSoundChannelVolume(int channel, int volume)
{
   // pack the op and the channel into the first byte
   stream->Write((OP_SOUND_VOLUME << 5) | channel);

   // waveform in the second byte
   stream->Write(volume);
}

void TempestIOStreamProxy::SetSoundChannelWaveform(int channel, int waveform)
{
   // pack the op and the channel into the first byte
   stream->Write((OP_SOUND_WAVE << 5) | channel);

   // waveform in the second byte
   stream->Write(waveform);
}

void TempestIOStreamProxy::Tick6KHz(void)
{
	// send just the opcode with no data
	stream->Write(OP_6KHZ_TICK << 5);

   // and check the return stream
   TempestInPacket packet;
   while (stream->Read(&packet))
   {
      this->isVectorHalt = (packet.flags1 & FLAG_VECTOR_HALT) != 0;
   }
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
   return isVectorHalt;
}

void TempestIOStreamProxy::VectorGo(void)
{
   // send just the opcode with no data
   stream->Write(OP_VECTOR_GO << 5);
   isVectorHalt = false;
}

void TempestIOStreamProxy::VectorReset(void)
{
	// send just the opcode with no data
	stream->Write(OP_VECTOR_RESET << 5);

   isVectorHalt = true;
}

