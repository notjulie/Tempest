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

#include "AbstractTempestDB.h"
#include "HighScoreList.h"
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
   uint32_t GetHighScore(int index) const { return highScores.GetScore(index); }
   uint8_t InsertHighScore(uint32_t score);
   void GetVectorData(VectorData &vectorData);
   void SetDemoMode(void) { demoMode = true; }
   void SetControlPanel(AbstractArcadeGameControlPanelReader *_controlPanel);
   void SetSoundOutput(AbstractSoundGenerator *soundOutput);

private:
   void  ConfigureAddressSpace(void);
   void  Tick250Hz(void);
   uint8_t ReadIOByte(uint16_t address);
   void    WriteIOByte(uint16_t address, uint8_t value);
   void Tick6KHz(void);

private:
   // our memory address handlers
   static uint8_t ReadColorRAM(AbstractBus *bus, uint16_t address);
   static uint8_t ReadHighScoreInitial(AbstractBus *bus, uint16_t address);
   static uint8_t ReadIO(AbstractBus *bus, uint16_t address);
   static uint8_t ReadPokey(AbstractBus *bus, uint16_t address);
   static uint8_t ReadVectorRAM(AbstractBus *bus, uint16_t address);
   static void WriteColorRAM(AbstractBus *bus, uint16_t address, uint8_t value);
   static void WriteHighScoreInitial(AbstractBus *bus, uint16_t address, uint8_t value);
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
   AbstractTempestDB   *db;

   AbstractTempestDB defaultDatabase;
   std::mutex vectorDataSnapshotMutex;
   VectorData vectorData;
   VectorData vectorDataSnapshot;
   TempestPokey1 pokey1;
   TempestPokey2 pokey2;
   MathBox mathBox;
   HighScoreList highScores;
   bool highScoresWritable = true;

   AbstractArcadeGameControlPanelReader *controlPanel = nullptr;
   AbstractSoundGenerator *soundOutput = nullptr;

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

