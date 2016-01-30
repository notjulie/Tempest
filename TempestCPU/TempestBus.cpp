
#include "stdafx.h"

#include "../TempestIO/AbstractTempestIO.h"

#include "AbstractTempestEnvironment.h"
#include "TempestException.h"
#include "TempestMemoryMap.h"
#include "TempestROMS.h"

#include "TempestBus.h"


TempestBus::TempestBus(AbstractTempestEnvironment *_environment)
	:
		mathBox(_environment)
{
	// clear
	selfTest = false;
	clock3KHzIsHigh = false;
	slam = false;
   tempestSoundIO = NULL;
   tempestVectorIO = NULL;

   mainRAM.resize(MAIN_RAM_SIZE);
   colorRAM.resize(COLOR_RAM_SIZE);

   // install our event for the 6KHz tick
   std::function<void()> f = [this]() { Tick6KHz(); };
   StartTimer(250, f);
}

TempestBus::~TempestBus(void)
{
}


uint8_t TempestBus::ReadByte(uint16_t address)
{
   // see if it's a ROM address
	if (address >= 0x9000)
	{
		switch (address & 0xF800)
		{
		case 0x9000:
			return ROM_136002_113[address & 0x7FF];
		case 0x9800:
			return ROM_136002_114[address & 0x7FF];
		case 0xA000:
			return ROM_136002_115[address & 0x7FF];
		case 0xA800:
			return ROM_136002_116[address & 0x7FF];
		case 0xB000:
			return ROM_136002_117[address & 0x7FF];
		case 0xB800:
			return ROM_136002_118[address & 0x7FF];
		case 0xC000:
		case 0xE000:
			return ROM_136002_119[address & 0x7FF];
		case 0xC800:
		case 0xE800:
			return ROM_136002_120[address & 0x7FF];
		case 0xD000:
		case 0xF000:
			return ROM_136002_121[address & 0x7FF];
		case 0xD800:
		case 0xF800:
			return ROM_136002_122[address & 0x7FF];
		}
	}

   // main RAM
   if (address >= MAIN_RAM_BASE && address < MAIN_RAM_BASE + MAIN_RAM_SIZE)
   {
      switch (address)
      {
      case 0x011F:
         // this is a value that gets set on startup that seems to be a
         // POKEY copy-protection thing that scrozzles things and causes
         // unpredictable lockups if it is not zero... so just force it to
         // zero
         return 0;

      default:
         return mainRAM[(unsigned)(address - MAIN_RAM_BASE)];
      }
   }

   // vector RAM
	if (IsVectorRAMAddress(address))
		return vectorRAM[address - VECTOR_RAM_BASE];

   // vector ROM
	if (address >= VECTOR_ROM_BASE && address < VECTOR_ROM_BASE + sizeof(ROM_136002_111))
		return ROM_136002_111[address - VECTOR_ROM_BASE];
	if (address >= VECTOR_ROM_BASE + sizeof(ROM_136002_111) && address < VECTOR_ROM_BASE + sizeof(ROM_136002_111) + sizeof(ROM_136002_112))
		return ROM_136002_112[address - VECTOR_ROM_BASE - sizeof(ROM_136002_111)];

   // POKEY 1
   if (address >= POKEY1_BASE && address <= POKEY1_END)
      return pokey1.ReadByte((uint16_t)(address - POKEY1_BASE));

   // POKEY 2
	if (address >= POKEY2_BASE && address <= POKEY2_END)
		return pokey2.ReadByte((uint16_t)(address - POKEY2_BASE));

	// color RAM
	if (address >= COLOR_RAM_BASE && address < COLOR_RAM_BASE + COLOR_RAM_SIZE)
		return colorRAM[(unsigned)(address - COLOR_RAM_BASE)];

	// miscellaneous other cases
   switch (address)
   {
		case 0x0C00:
		{
			uint8_t result = 0;
			if (clock3KHzIsHigh)
				result |= 0x80;
			if (tempestVectorIO->IsVectorHalt())
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
         char buffer[200];
         sprintf(buffer, "Invalid read address: %X", address);
         throw TempestException(buffer);
   }
}


void TempestBus::WriteByte(uint16_t address, uint8_t value)
{
   // main RAM
   if (address >= MAIN_RAM_BASE && address < MAIN_RAM_BASE + MAIN_RAM_SIZE)
   {
      mainRAM[(unsigned)(address - MAIN_RAM_BASE)] = value;
      return;
   }

   // vector RAM
   if (IsVectorRAMAddress(address))
   {
		tempestVectorIO->WriteVectorRAM((uint16_t)(address - VECTOR_RAM_BASE), value);
		vectorRAM[address - VECTOR_RAM_BASE] = value;
      return;
   }

   // color RAM
   if (address >= COLOR_RAM_BASE && address < COLOR_RAM_BASE + COLOR_RAM_SIZE)
   {
      tempestVectorIO->WriteColorRAM((uint16_t)(address - COLOR_RAM_BASE), value);
      colorRAM[(unsigned)(address - COLOR_RAM_BASE)] = value;
      return;
   }

   // POKEY 1
   if (address >= POKEY1_BASE && address <= POKEY1_END)
   {
      pokey1.WriteByte((uint16_t)(address - POKEY1_BASE), value);
      return;
   }

   // POKEY 2
   if (address >= POKEY2_BASE && address <= POKEY2_END)
   {
		pokey2.WriteByte((uint16_t)(address - POKEY2_BASE), value);
      return;
   }

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
         // vector state machine GO!
         tempestVectorIO->VectorGo();
         break;

      case 0x5000:
         // watchdog timer clear... this is also what clears the IRQ,
			// but we don't worry about either of those things
         break;

      case 0x5800:
         // vector state machine reset
         tempestVectorIO->VectorReset();
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
         char  buffer[200];
         sprintf(buffer, "Invalid write address: 0x%X", address);
         throw TempestException(buffer);
   }
}


void TempestBus::SetTempestIO(AbstractTempestSoundIO *_tempestSoundIO, AbstractTempestVectorIO *_tempestVectorIO)
{
   tempestSoundIO = _tempestSoundIO;
   tempestVectorIO = _tempestVectorIO;
   pokey1.SetTempestIO(tempestSoundIO);
	pokey2.SetTempestIO(tempestSoundIO);
}


bool TempestBus::IsVectorRAMAddress(uint16_t address)
{
	return address >= VECTOR_RAM_BASE && address<VECTOR_RAM_BASE + VECTOR_RAM_SIZE;
}


void TempestBus::Tick6KHz(void)
{
   clock3KHzIsHigh = !clock3KHzIsHigh;
   tempestSoundIO->Tick6KHz();

   std::function<void()> f = [this]() { Tick6KHz(); };
   StartTimer(250, f);
}
