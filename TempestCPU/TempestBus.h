/*
 * File:   TempestBus.h
 * Author: Randy
 *
 * Created on April 25, 2015, 1:50 PM
 */

#ifndef TEMPESTBUS_H
#define	TEMPESTBUS_H

#include <mutex>

#include "VectorData.h"

#include "6502/AbstractBus.h"
#include "MathBox/MathBox.h"

#include "EEPROM.h"
#include "TempestMemoryMap.h"
#include "TempestPokey.h"


// forward declarations
class AbstractTempestEnvironment;


#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

class TempestBus : public AbstractBus
{
public:
	TempestBus(AbstractTempestEnvironment *_environment);
	virtual ~TempestBus(void);

   void ClearWatchdog(void);
   void GetVectorData(VectorData &vectorData);
   bool IsPaused(void) { return isPaused; }
   void SetDemoMode(void) { demoMode = true; }
   void SetTempestIO(AbstractTempestSoundIO *tempestSoundIOs);

private:
   void  ConfigureAddressSpace(void);
   void  HandleTick250Hz(void);
   uint8_t ReadIOByte(uint16_t address);
   void    WriteIOByte(uint16_t address, uint8_t value);

private:
   static uint8_t ReadColorRAM(AbstractBus *bus, uint16_t address);
   static uint8_t ReadEEPROM(AbstractBus *bus, uint16_t address);
   static uint8_t ReadIO(AbstractBus *bus, uint16_t address);
   static uint8_t ReadPokey(AbstractBus *bus, uint16_t address);
   static uint8_t ReadVectorRAM(AbstractBus *bus, uint16_t address);
   static void WriteColorRAM(AbstractBus *bus, uint16_t address, uint8_t value);
   static void WriteEEPROM(AbstractBus *bus, uint16_t address, uint8_t value);
   static void WriteIO(AbstractBus *bus, uint16_t address, uint8_t value);
   static void WriteMathBoxValue(AbstractBus *bus, uint16_t address, uint8_t value);
   static void WritePokey(AbstractBus *bus, uint16_t address, uint8_t value);
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
   bool isPaused = false;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif	/* TEMPESTBUS_H */

