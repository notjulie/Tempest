/*
 * File:   TempestBus.h
 * Author: Randy
 *
 * Created on April 25, 2015, 1:50 PM
 */

#ifndef TEMPESTBUS_H
#define	TEMPESTBUS_H

#include "6502/AbstractBus.h"
#include "MathBox/MathBox.h"

#include "EEPROM.h"
#include "TempestMemoryMap.h"
#include "TempestPokey.h"

class AbstractTempestEnvironment;
class AbstractTempestVectorIO;
class VectorData;

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

class TempestBus : public AbstractBus
{
public:
	TempestBus(AbstractTempestEnvironment *_environment);
	virtual ~TempestBus(void);

   void SetDemoMode(void) { demoMode = true; }
   void SetTempestIO(AbstractTempestSoundIO *tempestSoundIO, AbstractTempestVectorIO *tempestVectorIO);

public:
	// AbstractBus overrides
	virtual uint8_t ReadByte(uint16_t address);
   virtual void    WriteByte(uint16_t address, uint8_t value);

private:
   void  HandleTick250Hz(void);

private:
	static bool		IsVectorRAMAddress(uint16_t address);
   static void Tick6KHz(AbstractBus *bus);
   static void Tick250Hz(AbstractBus *bus);

private:
	// forbidden items
	TempestBus(const TempestBus &bus);
	TempestBus &operator=(const TempestBus &bus);

private:
   AbstractTempestEnvironment *environment;

   std::vector<uint8_t>  mainRAM;
   std::vector<uint8_t>  colorRAM;
	uint8_t vectorRAM[VECTOR_RAM_SIZE];
   TempestPokey1 pokey1;
   TempestPokey2 pokey2;
   EEPROM eeprom;
   MathBox mathBox;

   AbstractTempestSoundIO *tempestSoundIO;
   AbstractTempestVectorIO *tempestVectorIO;

   uint64_t lastPlayer2ButtonDownTime;
   uint64_t lastWatchdogTime;
   bool demoMode;
	bool selfTest;
	bool slam;
	bool clock3KHzIsHigh;
	bool lastPlayer2ButtonState;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif	/* TEMPESTBUS_H */

