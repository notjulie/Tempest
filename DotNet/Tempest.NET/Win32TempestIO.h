
#ifndef WIN32TEMPESTIO_H
#define WIN32TEMPESTIO_H

#include "TempestIO/AbstractTempestIO.h"

#include "Win32WaveStreamer.h"


class Win32TempestSoundIO : public AbstractTempestSoundIO
{
public:
   Win32TempestSoundIO(void);
   virtual ~Win32TempestSoundIO(void);

   virtual void SetSoundChannelState(int channel, SoundChannelState state);
   virtual void SetTime(uint64_t clockCycles);
   virtual uint8_t GetButtons(void) { return buttons; }
   virtual uint8_t GetEncoder(void) { return encoder; }
   virtual void SetButtonLED(ButtonFlag button, bool value);

   void OnePlayer(bool state);
   void TwoPlayer(bool state);
   bool OnePlayerLED(void);
   bool TwoPlayerLED(void);
   void Fire(bool state);
   void Zap(bool state);
   void MoveWheel(int distance) { encoder += distance; }

private:
   Win32WaveStreamer	waveStreamer;
   uint8_t buttons;
   uint8_t encoder;
   uint8_t leds;
   uint64_t cpuTime;
};


#endif
