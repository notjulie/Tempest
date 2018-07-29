
#ifndef TEMPESTIOSTREAMPROXY_H
#define TEMPESTIOSTREAMPROXY_H

#include "AbstractSoundGenerator.h"
#include "AbstractTempestIO.h"
#include "PacketStream.h"
#include "SoundIOPacket.h"

class AbstractTempestStream;

class TempestIOStreamProxy :
   public AbstractSoundGenerator,
   public AbstractArcadeGameControlPanelReader 
{
public:
   TempestIOStreamProxy(AbstractTempestStream *stream);

	virtual void SetSoundChannelState(int channel, SoundChannelState state);
   virtual void SetButtonLED(ButtonFlag button, bool value);
   virtual uint8_t GetButtons(void) { return buttons; }
   virtual uint8_t GetEncoder(void) { return encoder; }

protected:
   virtual void ProcessTimeLapse(uint64_t clockCycles);

private:
   void StartPacket(void);
   void WriteSoundChannelState(const SoundChannelState &state);

private:
   PacketStream stream;
   uint8_t buttons;
   uint8_t encoder;
   uint64_t cpuTime;
   uint64_t lastSendTime;
   int ticksThisPacket;
   bool initialStatesSent;
   uint8_t leds;
   SoundChannelState channelState[8];
};

#endif
