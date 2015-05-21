
#include "stdafx.h"

#include "../TempestIO/AbstractTempestIO.h"

#include "AbstractTempestEnvironment.h"
#include "TempestException.h"
#include "TempestMemoryMap.h"

#include "TempestBus.h"


TempestBus::TempestBus(AbstractTempestEnvironment *_environment)
	:
		mathBox(_environment)
{
	// clear
	selfTest = false;
	clock3KHzIsHigh = false;
	slam = false;
	tempestIO = NULL;

   // create the ROM space
   rom.resize(20 * 1024);
   mainRAM.resize(MAIN_RAM_SIZE);
   colorRAM.resize(COLOR_RAM_SIZE);
}

TempestBus::~TempestBus(void)
{
}

bool TempestBus::HaveNewVectorData(void)
{
	return tempestIO->HaveNewVectorData();
}

void TempestBus::PopVectorData(VectorData &_vectorData)
{
	tempestIO->PopVectorData(_vectorData);
}


void TempestBus::LoadROM(const uint8_t *_rom, int length, uint16_t address)
{
	if (IsVectorROMAddress(address))
	{
		tempestIO->LoadVectorROM(address, _rom, length);
	}
	else
	{
		for (int i = 0; i<length; ++i)
			rom[(unsigned)(address - ROM_BASE + i)] = _rom[i];
	}
}

void TempestBus::LoadMathBoxROM(const uint8_t *rom, int length, char slot)
{
	mathBox.LoadROM(rom, length, slot);
}


uint8_t TempestBus::ReadByte(uint16_t address)
{
   // see if it's a ROM address
   if (address >= ROM_BASE && address < ROM_BASE + rom.size())
		return rom[(unsigned)(address - ROM_BASE)];
   
   // main RAM
   if (address >= MAIN_RAM_BASE && address < MAIN_RAM_BASE + MAIN_RAM_SIZE)
		return mainRAM[(unsigned)(address - MAIN_RAM_BASE)];

   // vector RAM
   if (IsVectorRAMAddress(address))
      return tempestIO->ReadVectorRAM(address);
   
   // vector ROM
   if (IsVectorROMAddress(address))
      return tempestIO->ReadVectorROM(address);
   
   // POKEY 1
   if (address >= POKEY1_BASE && address <= POKEY1_END)
      return pokey1.ReadByte((uint16_t)(address - POKEY1_BASE));
   
   // POKEY 2
	if (address >= POKEY2_BASE && address <= POKEY2_END)
		return pokey2.ReadByte((uint16_t)(address - POKEY2_BASE));
   
   // see if it's in the address range above ROM that acts like a
   // copy of high ROM
   if (address >= ROM_ECHO_START && address<= ROM_ECHO_END)
      return rom[(unsigned)((ROM_ECHO_SOURCE - ROM_BASE) + (address - ROM_ECHO_START))];
   
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
			if (tempestIO->IsVectorHalt())
				result |= 0x40;
			if (!selfTest)
				result |= 0x10;
			if (!slam)
				result |= 0x08;
			return result;
		}

      case 0x0D00:
         // DIP switch N13
			// low two bits==2 --> free play
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
         sprintf_s(buffer, "Invalid read address: %X", address);
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
      tempestIO->WriteVectorRAM(address, value);
      return;
   }
   
   // color RAM
   if (address >= COLOR_RAM_BASE && address < COLOR_RAM_BASE + COLOR_RAM_SIZE)
   {
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
         tempestIO->VectorGo();
         break;
         
      case 0x5000:
         // watchdog timer clear... this is also what clears the IRQ,
			// but we don't worry about either of those things
         break;
         
      case 0x5800:
         // vector state machine reset
         tempestIO->VectorReset();
         break;
         
      case 0x6040:
         // doc says mathbox status, readonly, but the code writes to it
         break;
         
      case 0x60E0:
         // register for flashing the start buttons
         break;
         
      default:
         char  buffer[200];
         sprintf_s(buffer, "Invalid write address: 0x%X", address);
         throw TempestException(buffer);
   }
}


void TempestBus::SetTempestIO(AbstractTempestIO *_tempestIO)
{
	tempestIO = _tempestIO;
	pokey1.SetTempestIO(tempestIO);
	pokey2.SetTempestIO(tempestIO);
}

void TempestBus::Toggle3KHzClock(void)
{
	clock3KHzIsHigh = !clock3KHzIsHigh;
	tempestIO->Tick6KHz();
}


bool TempestBus::IsVectorRAMAddress(uint16_t address)
{
	return address >= VECTOR_RAM_BASE && address<VECTOR_RAM_BASE + VECTOR_RAM_SIZE;
}


bool TempestBus::IsVectorROMAddress(uint16_t address)
{
	return address >= VECTOR_ROM_BASE && address<VECTOR_ROM_BASE + VECTOR_ROM_SIZE;
}

