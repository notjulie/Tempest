// ====================================================================
// Asteroids emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is the class that emulates the hardware of Asteroids that
//    the 6502 talks to directly, i.e. the RAM, ROM and I/O.
// ====================================================================

#ifndef ASTEROIDSBUS_H
#define	ASTEROIDSBUS_H

#include "TempestCPU/6502/AbstractBus.h"
#include "TempestIO/Vector/VectorData.h"


// forward declarations
class AbstractArcadeGameControlPanelReader;
class AbstractGameEnvironment;
class AbstractTempestSoundOutput;
class AsteroidsVectorInterpreter;
namespace std {
   // CLR code can't include <mutex>
   class mutex;
};


#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures
#endif

class AsteroidsBus : public AbstractBus
{
public:
   AsteroidsBus(AbstractGameEnvironment *_environment);
   virtual ~AsteroidsBus(void);
   AsteroidsBus(const AsteroidsBus &bus) = delete;
   AsteroidsBus &operator=(const AsteroidsBus &bus) = delete;

   void GetVectorData(AsteroidsVectorInterpreter &vectorInterpreter);
   bool IsVectorGo(void) const;
   void SetSoundOutput(AbstractTempestSoundOutput *soundOutput) { this->soundOutput = soundOutput; }
   void SetControlPanel(AbstractArcadeGameControlPanelReader *controlPanel) { this->controlPanel = controlPanel; }

private:
   void CheckVectorRAM(void);
   void ConfigureAddressSpace(void);
   void SetNMITimer(void);
   void Tick6KHz(void);

private:
   static uint8_t ReadBankedRAM(AbstractBus *bus, uint16_t address);
   static uint8_t ReadFireButton(AbstractBus *bus, uint16_t address) { return 0; }
   static uint8_t ReadHyperspaceButton(AbstractBus *bus, uint16_t address) { return 0; }
   static uint8_t ReadRotateLeftButton(AbstractBus *bus, uint16_t address);
   static uint8_t ReadRotateRightButton(AbstractBus *bus, uint16_t address);
   static uint8_t ReadThrustButton(AbstractBus *bus, uint16_t address) { return 0; }
   static uint8_t ReadOnePlayerButton(AbstractBus *bus, uint16_t address);
   static uint8_t ReadTwoPlayerButton(AbstractBus *bus, uint16_t address);
   static uint8_t ReadVectorRAM(AbstractBus *bus, uint16_t address);
   static uint8_t Read3KHzClock(AbstractBus *bus, uint16_t address);
   static uint8_t ReadSelfTestSwitch(AbstractBus *bus, uint16_t address);
   static void WriteBankedRAM(AbstractBus *bus, uint16_t address, uint8_t value);
   static void Write3200(AbstractBus *bus, uint16_t address, uint8_t value);
   static void WriteVectorRAM(AbstractBus *bus, uint16_t address, uint8_t value);
   static void WriteExplosionOutput(AbstractBus *bus, uint16_t address, uint8_t value) {}
   static void WriteThumpOutput(AbstractBus *bus, uint16_t address, uint8_t value) {}
   static void WriteResetSound(AbstractBus *bus, uint16_t address, uint8_t value);
   static void WriteSaucerSoundEnable(AbstractBus *bus, uint16_t address, uint8_t value) {}
   static void WriteSaucerFireSound(AbstractBus *bus, uint16_t address, uint8_t value) {}
   static void WriteSaucerSoundSelect(AbstractBus *bus, uint16_t address, uint8_t value) {}
   static void WriteThrustSoundEnable(AbstractBus *bus, uint16_t address, uint8_t value) {}
   static void WriteShipFireSound(AbstractBus *bus, uint16_t address, uint8_t value) {}
   static void WriteLifeSound(AbstractBus *bus, uint16_t address, uint8_t value) {}
   static void WriteVectorGO(AbstractBus *bus, uint16_t address, uint8_t value);

private:
   AbstractGameEnvironment *environment = nullptr;
   AbstractTempestSoundOutput *soundOutput = nullptr;
   AbstractArcadeGameControlPanelReader *controlPanel = nullptr;

   uint8_t bankedRAM[0x200];

   bool vectorGo = false;
   std::mutex *vectorDataSnapshotMutex = nullptr;
   uint8_t vectorRAM[0x800];
   uint8_t vectorRAMSnapshot[0x800];
   uint64_t lastVectorRAMWrite = 0;
   uint64_t lastVectorSnapshotTime = 0;

   bool ramSel = false;
   bool selfTest = false;
};

#ifdef _WIN32
#pragma warning(pop)
#endif

#endif	/* TEMPESTBUS_H */

