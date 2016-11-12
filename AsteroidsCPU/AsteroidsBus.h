#ifndef ASTEROIDSBUS_H
#define	ASTEROIDSBUS_H

#include "TempestCPU/6502/AbstractBus.h"
#include "TempestIO/Vector/VectorData.h"


// forward declarations
class AbstractTempestEnvironment;
class AbstractTempestSoundIO;
class AsteroidsVSM;
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
   AsteroidsBus(AbstractTempestEnvironment *_environment);
   virtual ~AsteroidsBus(void);
   AsteroidsBus(const AsteroidsBus &bus) = delete;
   AsteroidsBus &operator=(const AsteroidsBus &bus) = delete;

   void GetVectorData(AsteroidsVSM &vectorInterpreter);
   bool IsVectorGo(void) const { return vectorGo; }
   void SetTempestIO(AbstractTempestSoundIO *tempestSoundIOs);

private:
   void  ConfigureAddressSpace(void);
   void  HandleTick250Hz(void);

private:
   static uint8_t ReadBankedRAM(AbstractBus *bus, uint16_t address);
   static uint8_t ReadFireButton(AbstractBus *bus, uint16_t address) { return 0; }
   static uint8_t ReadHyperspaceButton(AbstractBus *bus, uint16_t address) { return 0; }
   static uint8_t ReadRotateLeftButton(AbstractBus *bus, uint16_t address) { return 0; }
   static uint8_t ReadRotateRightButton(AbstractBus *bus, uint16_t address) { return 0; }
   static uint8_t ReadThrustButton(AbstractBus *bus, uint16_t address) { return 0; }
   static uint8_t ReadOnePlayerButton(AbstractBus *bus, uint16_t address) { return 0; }
   static uint8_t ReadTwoPlayerButton(AbstractBus *bus, uint16_t address) { return 0; }
   static uint8_t ReadVectorRAM(AbstractBus *bus, uint16_t address);
   static uint8_t Read3KHzClock(AbstractBus *bus, uint16_t address);
   static uint8_t ReadSelfTestSwitch(AbstractBus *bus, uint16_t address);
   static void WriteBankedRAM(AbstractBus *bus, uint16_t address, uint8_t value);
   static void Write3200(AbstractBus *bus, uint16_t address, uint8_t value);
   static void WriteVectorRAM(AbstractBus *bus, uint16_t address, uint8_t value);
   static void WriteExplosionOutput(AbstractBus *bus, uint16_t address, uint8_t value) {}
   static void WriteThumpOutput(AbstractBus *bus, uint16_t address, uint8_t value) {}
   static void WriteSaucerSoundEnable(AbstractBus *bus, uint16_t address, uint8_t value) {}
   static void WriteSaucerFireSound(AbstractBus *bus, uint16_t address, uint8_t value) {}
   static void WriteSaucerSoundSelect(AbstractBus *bus, uint16_t address, uint8_t value) {}
   static void WriteThrustSoundEnable(AbstractBus *bus, uint16_t address, uint8_t value) {}
   static void WriteShipFireSound(AbstractBus *bus, uint16_t address, uint8_t value) {}
   static void WriteLifeSound(AbstractBus *bus, uint16_t address, uint8_t value) {}
   static void WriteVectorGO(AbstractBus *bus, uint16_t address, uint8_t value);

private:
   static void SetNMI(AbstractBus *bus);

private:
   AbstractTempestEnvironment *environment;

   std::mutex *vectorDataSnapshotMutex;

   AbstractTempestSoundIO *tempestSoundIO;

   uint8_t bankedRAM[0x200];
   uint8_t vectorRAM[0x800];

   bool ramSel = false;
   bool vectorGo = false;
   bool selfTest = false;
};

#ifdef _WIN32
#pragma warning(pop)
#endif

#endif	/* TEMPESTBUS_H */

