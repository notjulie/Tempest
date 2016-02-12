
#ifndef WIN32TEMPESTIO_H
#define WIN32TEMPESTIO_H

#include "TempestIO/AbstractTempestIO.h"
#include "TempestIO/Vector/SimpleVectorDataInterpreter.h"

#include "Win32WaveStreamer.h"

struct SimpleVector;


class Win32TempestVectorIO : public AbstractTempestVectorIO
{
public:
   Win32TempestVectorIO(void);
   virtual ~Win32TempestVectorIO(void);

   void GetVectorList(std::vector<SimpleVector> &_vectorList);

   virtual void WriteVectorRAM(uint16_t address, uint8_t value, uint64_t cpuTime) { vectorInterpreter.WriteVectorRAM(address, value); }
   virtual void WriteColorRAM(uint16_t address, uint8_t value) { vectorInterpreter.WriteColorRAM(address, value); }
   virtual bool IsVectorHalt(void) { return vectorInterpreter.IsHalt(); }
   virtual void VectorGo(void) { vectorInterpreter.Go(); }
   virtual void VectorReset(void) { vectorInterpreter.Reset(); }

private:
   void VectorThread(void);

private:
   static long VectorThreadEntry(Win32TempestVectorIO *pThis) { pThis->VectorThread(); return 0; }

private:
   HANDLE vectorThread;
   HANDLE vectorGeneratorMutex;
   DWORD vectorThreadID;
   bool terminating;
   SimpleVectorDataInterpreter	vectorInterpreter;
   std::vector<SimpleVector> currentVectors;
};


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
