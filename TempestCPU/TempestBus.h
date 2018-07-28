/*
 * File:   TempestBus.h
 * Author: Randy
 *
 * Created on April 25, 2015, 1:50 PM
 */

#ifndef TEMPESTBUS_H
#define	TEMPESTBUS_H

#include <mutex>
#include "6502/AbstractBus.h"

#include "EEPROM.h"
#include "MathBox.h"
#include "TempestMemoryMap.h"
#include "TempestPokey.h"
#include "VectorData.h"


// forward declarations
class AbstractGameEnvironment;


#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

class TempestBus : public AbstractBus
{
public:
	TempestBus(AbstractGameEnvironment *_environment);
	virtual ~TempestBus(void);

   void ClearWatchdog(void);
   void GetVectorData(VectorData &vectorData);
   void SetDemoMode(void) { demoMode = true; }
   void SetControlPanel(AbstractArcadeGameControlPanelReader *_controlPanel);
   void SetSoundOutput(AbstractTempestSoundOutput *soundOutput);

private:
   void  ConfigureAddressSpace(void);
   void  Tick250Hz(void);
   uint8_t ReadIOByte(uint16_t address);
   void    WriteIOByte(uint16_t address, uint8_t value);
   void Tick6KHz(void);

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
	// forbidden items
	TempestBus(const TempestBus &bus) = delete;
	TempestBus &operator=(const TempestBus &bus) = delete;

private:
   AbstractGameEnvironment *environment;

   std::mutex vectorDataSnapshotMutex;
   VectorData vectorData;
   VectorData vectorDataSnapshot;
   TempestPokey1 pokey1;
   TempestPokey2 pokey2;
   EEPROM eeprom;
   MathBox mathBox;

   AbstractArcadeGameControlPanelReader *controlPanel = nullptr;
   AbstractTempestSoundOutput *soundOutput = nullptr;

   uint64_t lastPlayer2ButtonDownTime = 0;
   uint64_t lastWatchdogTime = 0;
   uint64_t lastVectorRAMWrite = 0;
   bool demoMode = false;
	bool selfTest = false;
	bool slam = false;
	bool clock3KHzIsHigh = false;
   bool vectorGoRequested = false;
   bool vectorRAMReady = false;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif	/* TEMPESTBUS_H */

