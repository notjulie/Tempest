

#include "stdafx.h"

#include "TempestIO/AbstractTempestIO.h"

#include "TempestCPU/AbstractTempestEnvironment.h"
#include "TempestCPU/TempestException.h"

#include "AsteroidsROMs.h"
#include "AsteroidsVectorInterpreter.h"

#include "AsteroidsBus.h"


AsteroidsBus::AsteroidsBus(AbstractTempestEnvironment *_environment)
{
   // copy parameters
   environment = _environment;

   // allocate
   vectorDataSnapshotMutex = new std::mutex();

   // install our timers
   StartTimer(4000, &SetNMI);

   // configure address space
   ConfigureAddressSpace();
}

AsteroidsBus::~AsteroidsBus(void)
{
   delete vectorDataSnapshotMutex, vectorDataSnapshotMutex = NULL;
}


void AsteroidsBus::GetVectorData(AsteroidsVectorInterpreter &vectorData)
{
   vectorData.SetVectorRAM(vectorRAM);
}


void AsteroidsBus::SetTempestIO(AbstractTempestSoundIO *_tempestSoundIO)
{
   tempestSoundIO = _tempestSoundIO;
}


void AsteroidsBus::SetNMI(AbstractBus *bus)
{
   // generate the NMI
   bus->SetNMI();

   // this happens every 4ms so it's a good place to synch up with real time
   AsteroidsBus *pThis = static_cast<AsteroidsBus *>(bus);
   pThis->environment->SynchronizeClock(pThis->GetTotalClockCycles() / 1500);
}

/*void AsteroidsBus::Tick6KHz(AbstractBus *bus)
{
   AsteroidsBus *pThis = static_cast<AsteroidsBus *>(bus);

   // toggle the 3 KHz clock
   pThis->clock3KHzIsHigh = !pThis->clock3KHzIsHigh;

   // give the sound output its heartbeat
   pThis->tempestSoundIO->SetTime(pThis->GetTotalClockCycles());
}

void AsteroidsBus::Tick250Hz(AbstractBus *bus)
{
   AsteroidsBus *pThis = static_cast<AsteroidsBus *>(bus);
   pThis->HandleTick250Hz();
}

void AsteroidsBus::HandleTick250Hz(void)
{
   uint64_t now = GetTotalClockCycles();

   // check the watchdog
   if (now - lastWatchdogTime > 1500000)
      throw TempestException("Watchdog timer timeout");

   // check for double tap on player two button
   bool player2ButtonDown = (tempestSoundIO->GetButtons() & TWO_PLAYER_BUTTON) != 0;
   if (player2ButtonDown != lastPlayer2ButtonState)
   {
      lastPlayer2ButtonState = player2ButtonDown;
      if (player2ButtonDown)
      {
         // if this is a double tap we toggle the paused state
         if (now - lastPlayer2ButtonDownTime < 500000)
         {
            isPaused = !isPaused;
            if (isPaused)
               tempestSoundIO->AllSoundOff();
         }
         lastPlayer2ButtonDownTime = now;
      }
   }

   // synchronize the CPU with the realtime clock
   environment->SynchronizeClock(now / 1500);

   // generate an IRQ
   SetIRQ(true);
}*/


void AsteroidsBus::ConfigureAddressSpace(void)
{
   // burn all of our ROMs
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0x5000 + offset), ROM035127_02[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0x7800 + offset), ROM035143_02[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0x7000 + offset), ROM035144_02[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0x6800 + offset), ROM035145_02[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0xF800 + offset), ROM035143_02[offset]);

   // configure zero page RAM and the stack
   for (uint16_t address=0x0000; address<0x0200; ++address)
      ConfigureAddressAsRAM(address);

   // configure our banked pages
   for (uint16_t address=0x0200; address<0x0400; ++address)
      ConfigureAddress(address, 0, ReadBankedRAM, WriteBankedRAM);

   // configure vector RAM
   for (uint16_t address = 0x4000; address<0x4800; ++address)
      ConfigureAddress(address, 0, ReadVectorRAM, WriteVectorRAM);

   // configure memory mapped I/O
   ConfigureAddressAsROM(0x2002, 0x00); // vector HALT
   ConfigureAddressAsROM(0x2006, 0x00); // slam switch
   ConfigureAddressAsROM(0x2007, 0x00); // force the self-test switch off
   ConfigureAddressAsROM(0x2400, 0x00); // left coin switch
   ConfigureAddressAsROM(0x2401, 0x00); // center coin switch
   ConfigureAddressAsROM(0x2402, 0x00); // right coin switch
   ConfigureAddressAsROM(0x2403, 0x00); // one player start
   ConfigureAddressAsROM(0x2404, 0x00); // two player start
   ConfigureAddressAsROM(0x2800, 0x00); // sw 8&7, 0 = free play
   ConfigureAddressAsROM(0x2801, 0x00); // sw 6&5, coin options

   // it actually writes to 2802, but that's just because it does a rotate
   // followed by checking the carry... tolerate and ignore
   ConfigureAddress(0x2802, 0x00, ReadAddressNormal, WriteAddressNoOp); // sw 4&3, 0 = 4 ships
   ConfigureAddressAsROM(0x2803, 0x02); // sw 2&1, 2 = french
   ConfigureAddressAsRAM(0x3000); // vector GO
   ConfigureAddress(0x3200, 0, ReadAddressInvalid, Write3200);
   ConfigureAddressAsRAM(0x3400); // watchdog clear
   ConfigureAddress(0x3600, 0, ReadAddressInvalid, WriteExplosionOutput);
   ConfigureAddress(0x3A00, 0, ReadAddressInvalid, WriteThumpOutput);
   ConfigureAddress(0x3C00, 0, ReadAddressInvalid, WriteSaucerSoundEnable);
   ConfigureAddress(0x3C01, 0, ReadAddressInvalid, WriteSaucerFireSound);
   ConfigureAddress(0x3C02, 0, ReadAddressInvalid, WriteSaucerSoundSelect);
   ConfigureAddress(0x3C03, 0, ReadAddressInvalid, WriteThrustSoundEnable);
   ConfigureAddress(0x3C04, 0, ReadAddressInvalid, WriteShipFireSound);
   ConfigureAddress(0x3C05, 0, ReadAddressInvalid, WriteLifeSound);
}


uint8_t AsteroidsBus::ReadVectorRAM(AbstractBus *bus, uint16_t address)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);
   return asteroidsBus->vectorRAM[address - 0x4000];
}

void AsteroidsBus::WriteVectorRAM(AbstractBus *bus, uint16_t address, uint8_t value)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);
   asteroidsBus->vectorRAM[address - 0x4000] = value;
}

uint8_t AsteroidsBus::ReadBankedRAM(AbstractBus *bus, uint16_t address)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);
   address &= 0x1FF;
   if (asteroidsBus->ramSel)
      address ^= 0x100;
   return asteroidsBus->bankedRAM[address];
}

void AsteroidsBus::WriteBankedRAM(AbstractBus *bus, uint16_t address, uint8_t value)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);
   address &= 0x1FF;
   if (asteroidsBus->ramSel)
      address ^= 0x100;
   asteroidsBus->bankedRAM[address] = value;
}

void AsteroidsBus::Write3200(AbstractBus *bus, uint16_t address, uint8_t value)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);
   asteroidsBus->tempestSoundIO->SetButtonLED(ONE_PLAYER_BUTTON, (value & 2) != 0);
   asteroidsBus->tempestSoundIO->SetButtonLED(TWO_PLAYER_BUTTON, (value & 1) != 0);
   asteroidsBus->ramSel = (value & 4) != 0;
}

