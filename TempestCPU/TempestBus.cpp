
#include "stdafx.h"

#include "AbstractTempestIO.h"

#include "AbstractTempestEnvironment.h"
#include "TempestException.h"
#include "TempestMemoryMap.h"
#include "TempestROMS.h"

#include "TempestBus.h"


TempestBus::TempestBus(AbstractTempestEnvironment *_environment)
{
   // copy parameters
   environment = _environment;

	// clear
	selfTest = false;
	clock3KHzIsHigh = false;
	slam = false;
   tempestSoundIO = NULL;
   lastPlayer2ButtonState = false;
   lastPlayer2ButtonDownTime = 0;
   lastWatchdogTime = 0;
   lastVectorRAMWrite = 0;
   vectorGoRequested = false;
   vectorRAMReady = false;

   // allocate
   vectorDataSnapshotMutex = new std::mutex();

   // install our timers
   StartTimer(250, &Tick6KHz);
   StartTimer(6000, &Tick250Hz);

   // configure address space
   ConfigureAddressSpace();
}

TempestBus::~TempestBus(void)
{
   delete vectorDataSnapshotMutex, vectorDataSnapshotMutex = NULL;
}


void TempestBus::ClearWatchdog(void)
{
   SetIRQ(false);
   lastWatchdogTime = GetTotalClockCycles();
}


uint8_t TempestBus::ReadIOByte(uint16_t address)
{
	// miscellaneous IO bytes
   switch (address)
   {
		case 0x0C00:
		{
			uint8_t result = 0;
			if (clock3KHzIsHigh)
				result |= 0x80;
         // the Tempest app works just fine if it always thinks the vector
         // state machine is halted
         result |= 0x40;
			if (!selfTest)
				result |= 0x10;
			if (!slam)
				result |= 0x08;
			return result;
		}

      case 0x0D00:
         // DIP switch N13
         if (demoMode)
            return 0x42;

			// our default is low two bits==2 --> free play
         return 0x02;

      case 0x0E00:
         // DIP switch L12
         return 0;

      case 0x4000:
         //TODO register with the coin inputs
         return 0;

      case 0x6040:
         return mathBox.GetStatus();

      case 0x6050:
         return eeprom.ReadByte();

      case 0x6060:
         return mathBox.ReadLow();

      case 0x6070:
         return mathBox.ReadHigh();

      default:
      {
         char str[200];
         sprintf(str, "ReadIOByte; invalid address: %04X", address);
         throw TempestException(str);
      }
   }
}


void TempestBus::GetVectorData(VectorData &_vectorData)
{
   std::lock_guard<std::mutex> lock(*vectorDataSnapshotMutex);

   // if we don't have a valid snapshot yet just set the first instruction to be
   // a HALT instruction
   if (!vectorRAMReady)
      _vectorData.WriteVectorRAM(1, 0x20);
   else
      _vectorData = vectorDataSnapshot;
}


void TempestBus::WriteIOByte(uint16_t address, uint8_t value)
{
   // EEPROM
   if (address >= EEPROM_WRITE_BASE && address <= EEPROM_WRITE_END)
   {
		eeprom.WriteByte((uint16_t)(address - EEPROM_WRITE_BASE), value);
      return;
   }

   if (address >= MATHBOX_WRITE_BASE && address <= MATHBOX_WRITE_END)
   {
      mathBox.Write((uint8_t)(address - MATHBOX_WRITE_BASE), value);
      return;
   }

   // miscellaneous other cases
   switch (address)
   {
      case 0x4000:
         // video invert register
         break;

      case 0x4800:
         // vector state machine go
         vectorGoRequested = true;
         break;

      case 0x5000:
         // watchdog timer clear, and this is also what clears the IRQ
         ClearWatchdog();
         break;

      case 0x5800:
         // vector state machine reset... not needed
         break;

      case 0x6040:
         // doc says mathbox status, readonly, but the code writes to it
         break;

      case 0x60E0:
         // register for flashing the start buttons
         tempestSoundIO->SetButtonLED(ONE_PLAYER_BUTTON, (value & 1) != 0);
         tempestSoundIO->SetButtonLED(TWO_PLAYER_BUTTON, (value & 2) != 0);
         break;

      default:
      {
         char str[200];
         sprintf(str, "WriteIOByte; invalid address: %04X", address);
         throw TempestException(str);
      }
   }
}


void TempestBus::SetTempestIO(AbstractTempestSoundIO *_tempestSoundIO)
{
   tempestSoundIO = _tempestSoundIO;
   pokey1.SetTempestIO(tempestSoundIO);
	pokey2.SetTempestIO(tempestSoundIO);
}


void TempestBus::Tick6KHz(AbstractBus *bus)
{
   TempestBus *pThis = static_cast<TempestBus *>(bus);

   // toggle the 3 KHz clock
   pThis->clock3KHzIsHigh = !pThis->clock3KHzIsHigh;

   // give the sound output its heartbeat
   pThis->tempestSoundIO->SetTime(pThis->GetTotalClockCycles());
}

void TempestBus::Tick250Hz(AbstractBus *bus)
{
   TempestBus *pThis = static_cast<TempestBus *>(bus);
   pThis->HandleTick250Hz();
}

void TempestBus::HandleTick250Hz(void)
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
}


void TempestBus::ConfigureAddressSpace(void)
{
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
}


uint8_t TempestBus::ReadColorRAM(AbstractBus *bus, uint16_t address)
{
   TempestBus *tempestBus = static_cast<TempestBus *>(bus);
   return tempestBus->vectorData.ReadColorRAM((unsigned)(address - COLOR_RAM_BASE));
}

uint8_t TempestBus::ReadEEPROM(AbstractBus *bus, uint16_t)
{
   TempestBus *tempestBus = static_cast<TempestBus *>(bus);
   return tempestBus->eeprom.ReadByte();
}

uint8_t TempestBus::ReadIO(AbstractBus *bus, uint16_t address)
{
   TempestBus *tempestBus = static_cast<TempestBus *>(bus);
   return tempestBus->ReadIOByte(address);
}

uint8_t TempestBus::ReadPokey(AbstractBus *bus, uint16_t address)
{
   TempestBus *tempestBus = static_cast<TempestBus *>(bus);

   if (address >= POKEY1_BASE && address <= POKEY1_END)
      return tempestBus->pokey1.ReadByte((uint16_t)(address - POKEY1_BASE));
   else
      return tempestBus->pokey2.ReadByte((uint16_t)(address - POKEY2_BASE));
}

uint8_t TempestBus::ReadVectorRAM(AbstractBus *bus, uint16_t address)
{
   TempestBus *tempestBus = static_cast<TempestBus *>(bus);
   return tempestBus->vectorData.GetAt((uint16_t)(address - VECTOR_RAM_BASE));
}

void TempestBus::WriteColorRAM(AbstractBus *bus, uint16_t address, uint8_t value)
{
   TempestBus *tempestBus = static_cast<TempestBus *>(bus);
   tempestBus->vectorData.WriteColorRAM((uint16_t)(address - COLOR_RAM_BASE), value);
}

void TempestBus::WriteEEPROM(AbstractBus *bus, uint16_t address, uint8_t value)
{
   TempestBus *tempestBus = static_cast<TempestBus *>(bus);
   tempestBus->eeprom.WriteByte((uint16_t)(address - EEPROM_WRITE_BASE), value);
}

void TempestBus::WriteIO(AbstractBus *bus, uint16_t address, uint8_t value)
{
   TempestBus *tempestBus = static_cast<TempestBus *>(bus);
   tempestBus->WriteIOByte(address, value);
}

void TempestBus::WriteMathBoxValue(AbstractBus *bus, uint16_t address, uint8_t value)
{
   TempestBus *tempestBus = static_cast<TempestBus *>(bus);
   tempestBus->mathBox.Write((uint8_t)(address - MATHBOX_WRITE_BASE), value);
}

void TempestBus::WritePokey(AbstractBus *bus, uint16_t address, uint8_t value)
{
   TempestBus *tempestBus = static_cast<TempestBus *>(bus);

   if (address >= POKEY1_BASE && address <= POKEY1_END)
      tempestBus->pokey1.WriteByte((uint16_t)(address - POKEY1_BASE), value, tempestBus->GetTotalClockCycles());
   else
      tempestBus->pokey2.WriteByte((uint16_t)(address - POKEY2_BASE), value, tempestBus->GetTotalClockCycles());
}

void TempestBus::WriteVectorRAM(AbstractBus *bus, uint16_t address, uint8_t value)
{
   TempestBus *tempestBus = static_cast<TempestBus *>(bus);

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
}
