
#include "TempestCPU.Headers.h"

#include "AbstractGameEnvironment.h"
#include "AbstractSoundGenerator.h"
#include "AbstractTempestIO.h"

#include "TempestException.h"
#include "TempestMemoryMap.h"
#include "TempestROMS.h"

#include "TempestBus.h"


TempestBus::TempestBus(AbstractGameEnvironment *_environment)
{
   // copy parameters
   environment = _environment;
   
   // get our database from the environment; if they environment doesn't have one just use a default database
   // which is basically just a stub
   db = environment->GetResource<AbstractTempestDB *>("tempest.db");
   if (db == nullptr)
      db = &defaultDatabase;

   // install our timers
   StartTimer(250, [this]() { Tick6KHz(); });
   StartTimer(6000, [this]() { Tick250Hz(); });

   // configure address space
   ConfigureAddressSpace();

   // load high scores; if this succeeds we mark the high scores as read only so that the
   // game doesn't clear them on startup, else we allow the game to initialize them
   highScoresWritable = !db->LoadHighScores(highScores);
}

TempestBus::~TempestBus(void)
{
}


void TempestBus::ClearWatchdog(void)
{
   SetIRQ(false);
   lastWatchdogTime = GetTotalClockCycles();
}


uint8_t TempestBus::InsertHighScore(uint32_t score) 
{
   // note that we are now allowing the high scores to be written
   highScoresWritable = true;

   // insert the high score
   uint8_t result = highScores.InsertScore(score);

   // write the new scores to the database
   db->SaveHighScores(highScores);

   // return the position of the inserted score
   return result;
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
   std::lock_guard<std::mutex> lock(vectorDataSnapshotMutex);

   // if we don't have a valid snapshot yet just set the first instruction to be
   // a HALT instruction
   if (!vectorRAMReady)
      _vectorData.WriteVectorRAM(1, 0x20);
   else
      _vectorData = vectorDataSnapshot;
}


void TempestBus::WriteIOByte(uint16_t address, uint8_t value)
{
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
         controlPanel->SetButtonLED(ONE_PLAYER_BUTTON, (value & 1) != 0);
         controlPanel->SetButtonLED(TWO_PLAYER_BUTTON, (value & 2) != 0);
         break;

      default:
      {
         char str[200];
         sprintf(str, "WriteIOByte; invalid address: %04X", address);
         throw TempestException(str);
      }
   }
}


void TempestBus::SetSoundOutput(AbstractSoundGenerator *_soundOutput)
{
   this->soundOutput = _soundOutput;
   pokey1.SetTempestIO(_soundOutput);
	pokey2.SetTempestIO(_soundOutput);
}

void TempestBus::SetControlPanel(AbstractArcadeGameControlPanelReader *_controlPanel)
{
   this->controlPanel = _controlPanel;
   pokey1.SetControlPanel(_controlPanel);
   pokey2.SetControlPanel(_controlPanel);
}


void TempestBus::Tick6KHz(void)
{
   // toggle the 3 KHz clock
   clock3KHzIsHigh = !clock3KHzIsHigh;

   // give the sound output its heartbeat
   soundOutput->SetCPUTime(GetTotalClockCycles());
}

void TempestBus::Tick250Hz(void)
{
   uint64_t now = GetTotalClockCycles();

   // check the watchdog
   if (now - lastWatchdogTime > 1500000)
      throw TempestException("Watchdog timer timeout");

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

   // EEPROM... just treat it as RAM, because we don't use it
   for (uint16_t address = EEPROM_WRITE_BASE; address <= EEPROM_WRITE_END; ++address)
      ConfigureAddressAsRAM(address);
   ConfigureAddressAsRAM(EEPROM_READ);

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

   // point the memory addresses associated with high score initials to the high score table
   for (uint16_t i = HIGH_SCORE_INITIALS_START; i <= HIGH_SCORE_INITIALS_END; ++i)
      ConfigureAddress(i, 0, ReadHighScoreInitial, WriteHighScoreInitial);
}


uint8_t TempestBus::ReadColorRAM(AbstractBus *bus, uint16_t address)
{
   TempestBus *tempestBus = static_cast<TempestBus *>(bus);
   return tempestBus->vectorData.ReadColorRAM((unsigned)(address - COLOR_RAM_BASE));
}

uint8_t TempestBus::ReadHighScoreInitial(AbstractBus *bus, uint16_t address)
{
   TempestBus *tempestBus = static_cast<TempestBus *>(bus);
   int offset = HIGH_SCORE_INITIALS_END - address;
   return tempestBus->highScores.GetInitial(offset/3, offset%3).GetRawValue();
}

void TempestBus::WriteHighScoreInitial(AbstractBus *bus, uint16_t address, uint8_t value)
{
   TempestBus *tempestBus = static_cast<TempestBus *>(bus);
   if (tempestBus->highScoresWritable)
   {
      int offset = HIGH_SCORE_INITIALS_END - address;
      tempestBus->highScores.SetInitial(offset / 3, offset % 3, TempestChar::FromRawValue(value));
      tempestBus->db->SaveHighScores(tempestBus->highScores);
   }
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
      std::lock_guard<std::mutex> lock(tempestBus->vectorDataSnapshotMutex);

      tempestBus->vectorDataSnapshot = tempestBus->vectorData;
      if (tempestBus->vectorGoRequested)
         tempestBus->vectorRAMReady = true;
   }

   tempestBus->vectorData.WriteVectorRAM((uint16_t)(address - VECTOR_RAM_BASE), value);
   tempestBus->lastVectorRAMWrite = tempestBus->GetTotalClockCycles();
}
