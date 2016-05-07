/*
 * File:   TempestBus.h
 * Author: Randy
 *
 * Created on April 25, 2015, 1:50 PM
 */

#ifndef TEMPESTBUS_H
#define	TEMPESTBUS_H

#include "../TempestIO/Vector/VectorData.h"

#include "6502/AbstractBus.h"
#include "MathBox/MathBox.h"

#include "EEPROM.h"
#include "TempestMemoryMap.h"
#include "TempestPokey.h"


// forward declarations
class AbstractTempestEnvironment;
namespace std {
   // CLR code can't include <mutex>
   class mutex;
};


#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

class TempestBus : public AbstractBus
{
public:
	TempestBus(AbstractTempestEnvironment *_environment);
	virtual ~TempestBus(void);

   void GetVectorData(VectorData &vectorData);
   void SetDemoMode(void) { demoMode = true; }
   void SetTempestIO(AbstractTempestSoundIO *tempestSoundIOs);

public:
	// AbstractBus overrides
	virtual uint8_t ReadByte(uint16_t address);
   virtual void    WriteByte(uint16_t address, uint8_t value);

private:
   void  ConfigureAddressSpace(void);
   void  HandleTick250Hz(void);

private:
   static uint8_t ReadVectorRAM(AbstractBus *bus, uint16_t address);
   static void WriteVectorRAM(AbstractBus *bus, uint16_t address, uint8_t value);

private:
   static void Tick6KHz(AbstractBus *bus);
   static void Tick250Hz(AbstractBus *bus);

private:
	// forbidden items
	TempestBus(const TempestBus &bus);
	TempestBus &operator=(const TempestBus &bus);

private:
   AbstractTempestEnvironment *environment;

   VectorData vectorData;
   VectorData vectorDataSnapshot;
   std::mutex *vectorDataSnapshotMutex;
   TempestPokey1 pokey1;
   TempestPokey2 pokey2;
   EEPROM eeprom;
   MathBox mathBox;

   AbstractTempestSoundIO *tempestSoundIO;

   uint64_t lastPlayer2ButtonDownTime;
   uint64_t lastWatchdogTime;
   uint64_t lastVectorRAMWrite;
   bool demoMode;
	bool selfTest;
	bool slam;
	bool clock3KHzIsHigh;
	bool lastPlayer2ButtonState;
   bool vectorGoRequested;
   bool vectorRAMReady;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif	/* TEMPESTBUS_H */

