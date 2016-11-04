

#include "stdafx.h"

#include "TempestIO/AbstractTempestIO.h"

#include "TempestCPU/AbstractTempestEnvironment.h"
#include "TempestCPU/TempestException.h"
//#include "TempestMemoryMap.h"
#include "AsteroidsROMs.h"

#include "AsteroidsBus.h"


AsteroidsBus::AsteroidsBus(AbstractTempestEnvironment *_environment)
{
   // copy parameters
   environment = _environment;

   // allocate
   vectorDataSnapshotMutex = new std::mutex();

   // install our timers
   //StartTimer(250, &Tick6KHz);
   //StartTimer(6000, &Tick250Hz);

   // configure address space
   ConfigureAddressSpace();
}

AsteroidsBus::~AsteroidsBus(void)
{
   delete vectorDataSnapshotMutex, vectorDataSnapshotMutex = NULL;
}


void AsteroidsBus::GetVectorData(VectorData &vectorData)
{
/*   std::lock_guard<std::mutex> lock(*vectorDataSnapshotMutex);

   // if we don't have a valid snapshot yet just set the first instruction to be
   // a HALT instruction
   if (!vectorRAMReady)
      vectorData.WriteVectorRAM(1, 0x20);
   else
      vectorData = vectorDataSnapshot;*/
}


void AsteroidsBus::SetTempestIO(AbstractTempestSoundIO *_tempestSoundIO)
{
   tempestSoundIO = _tempestSoundIO;
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
      ConfigureAddressAsRAM(address);

   // configure memory mapped I/O
   ConfigureAddressAsROM(0x2007, 0x00); // force the self-test switch off
   ConfigureAddressAsROM(0x2800, 0x00); // sw 8&7, 0 = free play
   ConfigureAddressAsROM(0x2801, 0x00); // sw 6&5, coin options

   // it actually writes to 2802, but that's just because it does a rotate
   // followed by checking the carry... tolerate and ignore
   ConfigureAddress(0x2802, 0x00, ReadAddressNormal, WriteAddressNoOp); // sw 4&3, 0 = 4 ships
   ConfigureAddressAsROM(0x2803, 0x02); // sw 2&1, 2 = french
   ConfigureAddress(0x3200, 0, ReadAddressInvalid, Write3200);
   ConfigureAddress(0x3600, 0, ReadAddressInvalid, WriteExplosionOutput);
   ConfigureAddress(0x3A00, 0, ReadAddressInvalid, WriteThumpOutput);
   ConfigureAddress(0x3C00, 0, ReadAddressInvalid, WriteSaucerSoundEnable);
   ConfigureAddress(0x3C01, 0, ReadAddressInvalid, WriteSaucerFireSound);
   ConfigureAddress(0x3C02, 0, ReadAddressInvalid, WriteSaucerSoundSelect);
   ConfigureAddress(0x3C03, 0, ReadAddressInvalid, WriteThrustSoundEnable);
   ConfigureAddress(0x3C04, 0, ReadAddressInvalid, WriteShipFireSound);
   ConfigureAddress(0x3C05, 0, ReadAddressInvalid, WriteLifeSound);

#if 0
   // burn all of our ROMs
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0x9000 + offset), ROM_136002_113[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0x9800 + offset), ROM_136002_114[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0xA000 + offset), ROM_136002_115[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0xA800 + offset), ROM_136002_116[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0xB000 + offset), ROM_136002_117[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0xB800 + offset), ROM_136002_118[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0xC000 + offset), ROM_136002_119[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0xC800 + offset), ROM_136002_120[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0xD000 + offset), ROM_136002_121[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0xD800 + offset), ROM_136002_122[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0xE000 + offset), ROM_136002_119[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0xE800 + offset), ROM_136002_120[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0xF000 + offset), ROM_136002_121[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0xF800 + offset), ROM_136002_122[offset]);

   // the vector ROMs, too
   for (uint16_t offset = 0; offset < sizeof(ROM_136002_111); ++offset)
      ConfigureAddressAsROM((uint16_t)(VECTOR_ROM_BASE + offset), ROM_136002_111[offset]);
   for (uint16_t offset = 0; offset < sizeof(ROM_136002_112); ++offset)
      ConfigureAddressAsROM((uint16_t)(VECTOR_ROM_BASE + sizeof(ROM_136002_111) + offset), ROM_136002_112[offset]);

   // configure main RAM as RAM
   for (unsigned i = 0; i < MAIN_RAM_SIZE; ++i)
      ConfigureAddressAsRAM((uint16_t)(MAIN_RAM_BASE + i));

   // set up vector RAM and color RAM
   for (uint16_t offset = 0; offset < VECTOR_RAM_SIZE; ++offset)
      ConfigureAddress((uint16_t)(VECTOR_RAM_BASE + offset), 0, ReadVectorRAM, WriteVectorRAM);
   for (uint16_t offset = 0; offset < COLOR_RAM_SIZE; ++offset)
      ConfigureAddress((uint16_t)(COLOR_RAM_BASE + offset), 0, ReadColorRAM, WriteColorRAM);

   // the Pokeys
   for (uint16_t address = POKEY1_BASE; address <= POKEY1_END; ++address)
      ConfigureAddress(address, 0, ReadPokey, WritePokey);
   for (uint16_t address = POKEY2_BASE; address <= POKEY2_END; ++address)
      ConfigureAddress(address, 0, ReadPokey, WritePokey);

   // EEPROM
   for (uint16_t address = EEPROM_WRITE_BASE; address <= EEPROM_WRITE_END; ++address)
      ConfigureAddress(address, 0, ReadAddressInvalid, WriteEEPROM);
   ConfigureAddress(0x6050, 0, ReadEEPROM, WriteAddressInvalid);

   // Mathbox
   for (uint16_t address = MATHBOX_WRITE_BASE; address <= MATHBOX_WRITE_END; ++address)
      ConfigureAddress(address, 0, ReadAddressInvalid, WriteMathBoxValue);

   // and our miscellaneous IO
   static const uint16_t ioAddresses[] = {
      0x0C00,
      0x0D00,
      0x0E00,
      0x4000,
      0x4800,
      0x5000,
      0x5800,
      0x6040,
      0x6050,
      0x6060,
      0x6070,
      0x60E0
   };
   for (unsigned i = 0; i < sizeof(ioAddresses) / sizeof(ioAddresses[0]); ++i)
      ConfigureAddress(ioAddresses[i], 0, ReadIO, WriteIO);

   // this is a main RAM address that gets set on startup that seems to be a
   // POKEY copy-protection thing that scrozzles things and causes
   // unpredictable lockups if it is not zero... so just force it to
   // zero
   ConfigureAddress(COPY_PROTECTION_FLAG_ADDRESS, 0, ReadAddressNormal, WriteAddressNoOp);

   // if I set this to a fixed value I can start on level 80 without the other features
   // of demo mode enabled
   ConfigureAddress(MAX_START_LEVEL_ADDRESS, 80, ReadAddressNormal, WriteAddressNoOp);
#endif
}


uint8_t AsteroidsBus::ReadVectorRAM(AbstractBus *bus, uint16_t address)
{
   AsteroidsBus *tempestBus = static_cast<AsteroidsBus *>(bus);
   return tempestBus->vectorData.GetAt((uint16_t)(address - VECTOR_RAM_BASE));
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

/*void AsteroidsBus::WriteVectorRAM(AbstractBus *bus, uint16_t address, uint8_t value)
{
   AsteroidsBus *tempestBus = static_cast<AsteroidsBus *>(bus);

   // If it has been a while (as measured in clock cycles) since the last write to
   // vector RAM we can conclude that it is stable and take a snapshot of it so that
   // the display doesn't catch it in the middle of updating; 800 clock ticks appears
   // to be a trustworthy number.  In the near future I expect to locate a particular
   // place in the 6502 code that I can use instead as a trigger.
   if (tempestBus->GetTotalClockCycles() - tempestBus->lastVectorRAMWrite > 800)
   {
      std::lock_guard<std::mutex> lock(*tempestBus->vectorDataSnapshotMutex);

      tempestBus->vectorDataSnapshot = tempestBus->vectorData;
      tempestBus->lastVectorRAMWrite = tempestBus->GetTotalClockCycles();
      if (tempestBus->vectorGoRequested)
         tempestBus->vectorRAMReady = true;
   }

   tempestBus->vectorData.WriteVectorRAM((uint16_t)(address - VECTOR_RAM_BASE), value);
}*/

