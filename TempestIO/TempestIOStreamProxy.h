
#ifndef TEMPESTIOSTREAMPROXY_H
#define TEMPESTIOSTREAMPROXY_H

#include "AbstractTempestIO.h"
#include "PacketStream.h"
#include "SoundIOPacket.h"

class AbstractTempestStream;

class TempestIOStreamProxy : public AbstractTempestSoundIO
{
public:
   TempestIOStreamProxy(AbstractTempestStream *stream);

	virtual void SetSoundChannelFrequency(int channel, int frequency);
	virtual void SetSoundChannelVolume(int channel, int volume);
	virtual void SetSoundChannelWaveform(int channel, int waveform);
   virtual void SetButtonLED(ButtonFlag button, bool value);
   virtual void Delay(int clockCycles);
   virtual uint8_t GetButtons(void) { return buttons; }
   virtual uint8_t GetEncoder(void) { return encoder; }

private:
   PacketStream stream;
   SoundIOPacket currentState;
   uint8_t buttons;
   uint8_t encoder;
   int clockCyclesSinceLastSend;
};

#endif
