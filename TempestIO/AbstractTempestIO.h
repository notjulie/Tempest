
#ifndef ABSTRACTTEMPESTIO_H
#define ABSTRACTTEMPESTIO_H

class VectorData;

enum ButtonFlag {
   ONE_PLAYER_BUTTON = 1,
   FIRE_BUTTON = 2,
   ZAPPER_BUTTON = 4,
   ENCODER_UP = 8,
   ENCODER_DOWN = 16,
   TWO_PLAYER_BUTTON = 32
};

class AbstractTempestSoundIO
{
public:
   virtual ~AbstractTempestSoundIO(void) {}

	virtual void SetSoundChannelFrequency(int channel, int frequency) = 0;
	virtual void SetSoundChannelVolume(int channel, int volume) = 0;
	virtual void SetSoundChannelWaveform(int channel, int waveform) = 0;
	virtual void Delay(int clockCycles) = 0;
   virtual uint8_t GetButtons(void) = 0;
   virtual uint8_t GetEncoder(void) = 0;
   virtual void SetButtonLED(ButtonFlag button, bool value) = 0;

   void AllSoundOff(void) {
      for (int i=0; i<8; ++i)
         SetSoundChannelVolume(i, 0);
   }
};

class AbstractTempestVectorIO
{
public:
   virtual ~AbstractTempestVectorIO(void) {}

   virtual void WriteColorRAM(uint16_t address, uint8_t value) = 0;
   virtual void WriteVectorRAM(uint16_t address, uint8_t value, uint64_t cpuTime) = 0;
   virtual bool IsVectorHalt(void) = 0;
   virtual void VectorGo(void) = 0;
   virtual void VectorReset(void) = 0;
};


#endif
