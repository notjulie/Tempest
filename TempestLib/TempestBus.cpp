
#include <stdio.h>

#include "Abstract3KHzClock.h"
#include "TempestException.h"

#include "TempestBus.h"

static const uint16_t ROM_BASE = 0x9000;
static const uint16_t ROM_ECHO_START = 0xE000;
static const uint16_t ROM_ECHO_END = 0xFFFF;
static const uint16_t ROM_ECHO_SOURCE = 0xC000;
static const uint16_t MAIN_RAM_BASE = 0x0;
static const uint16_t MAIN_RAM_SIZE = 0x800;
static const uint16_t COLOR_RAM_BASE = 0x0800;
static const uint16_t COLOR_RAM_SIZE = 0x0010;
static const uint16_t POKEY1_BASE = 0x60C0;
static const uint16_t POKEY1_END = 0x60CF;
static const uint16_t POKEY2_BASE = 0x60D0;
static const uint16_t POKEY2_END = 0x60DF;
static const uint16_t EEPROM_WRITE_BASE = 0x6000;
static const uint16_t EEPROM_WRITE_END = 0x603F;
static const uint16_t MATHBOX_WRITE_BASE = 0x6080;
static const uint16_t MATHBOX_WRITE_END = 0x609F;

TempestBus::TempestBus(Abstract3KHzClock *_clock3KHz)
{
   // save parameters
   clock3KHz = _clock3KHz;
   
   // create the ROM space
   rom.resize(20 * 1024);
   mainRAM.resize(MAIN_RAM_SIZE);
   colorRAM.resize(COLOR_RAM_SIZE);
}

void TempestBus::LoadROM(const std::string &_path, uint16_t address)
{
   // the file is supposed to be 2K long... that's what we expect
   // for now
   FILE *f = fopen(_path.c_str(), "rb");
   if (f == NULL)
      throw TempestException(std::string("Can't open ROM file: '") + _path + "'");
   
   try
   {
      uint8_t  buffer[2048];
      int bytesRead = fread(buffer, 1, 2048, f);
      if (bytesRead != 2048)
         throw TempestException(std::string("File too short: '") + _path + "'");
      
      uint8_t  buffer2[1];
      int bytesRead2 = fread(buffer2, 1, 1, f);
      if (bytesRead2 != 0)
         throw TempestException(std::string("File too long: '") + _path + "'");

	  LoadROM(buffer, bytesRead, address);
      
      fclose(f);
   }
   catch (...)
   {
      fclose(f);
      throw;
   }
}

bool TempestBus::HaveNewVectorData(void)
{
	return vectorGenerator.HaveNewData();
}

void TempestBus::PopVectorData(VectorData &_vectorData)
{
	vectorGenerator.Pop(_vectorData);
}


void TempestBus::LoadROM(const uint8_t *_rom, int length, uint16_t address)
{
	if (vectorGenerator.IsVectorROMAddress(address))
	{
		vectorGenerator.LoadROM(address, _rom, length);
	}
	else
	{
		for (int i = 0; i<length; ++i)
			rom[address - ROM_BASE + i] = _rom[i];
	}
}

uint8_t TempestBus::ReadByte(uint16_t address)
{
   // see if it's a ROM address
   if (address >= ROM_BASE && address < ROM_BASE + rom.size())
      return rom[address - ROM_BASE];
   
   // main RAM
   if (address >= MAIN_RAM_BASE && address < MAIN_RAM_BASE + MAIN_RAM_SIZE)
      return mainRAM[address - MAIN_RAM_BASE];

   // vector RAM
   if (vectorGenerator.IsVectorRAMAddress(address))
      return vectorGenerator.ReadVectorRAM(address);
   
   // vector ROM
   if (vectorGenerator.IsVectorROMAddress(address))
      return vectorGenerator.ReadVectorROM(address);
   
   // POKEY 1
   if (address >= POKEY1_BASE && address <= POKEY1_END)
      return pokey1.ReadByte(address - POKEY1_BASE);
   
   // POKEY 2
   if (address >= POKEY2_BASE && address <= POKEY2_END)
      return pokey2.ReadByte(address - POKEY2_BASE);
   
   // see if it's in the address range above ROM that acts like a
   // copy of high ROM
   if (address >= ROM_ECHO_START && address<= ROM_ECHO_END)
      return rom[(ROM_ECHO_SOURCE - ROM_BASE) + (address - ROM_ECHO_START)];
   
   // miscellaneous other cases
   switch (address)
   {
      case 0x0C00:
         return this->clock3KHz->IsHigh() ? 0x80 : 0x00;

      case 0x0D00:
         // DIP switch N13
         return 0;
         
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
         return mathBox.Read1();
         
      case 0x6070:
         return mathBox.Read2();
         
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
      mainRAM[address - MAIN_RAM_BASE] = value;
      return;
   }
   
   // vector RAM
   if (vectorGenerator.IsVectorRAMAddress(address))
   {
      vectorGenerator.WriteVectorRAM(address, value);
      return;
   }
   
   // color RAM
   if (address >= COLOR_RAM_BASE && address < COLOR_RAM_BASE + COLOR_RAM_SIZE)
   {
      colorRAM[address - COLOR_RAM_BASE] = value;
      return;
   }
   
   // POKEY 1
   if (address >= POKEY1_BASE && address <= POKEY1_END)
   {
      pokey1.WriteByte(address - POKEY1_BASE, value);
      return;
   }
   
   // POKEY 2
   if (address >= POKEY2_BASE && address <= POKEY2_END)
   {
      pokey2.WriteByte(address - POKEY2_BASE, value);
      return;
   }
   
   // EEPROM
   if (address >= EEPROM_WRITE_BASE && address <= EEPROM_WRITE_END)
   {
      eeprom.WriteByte(address - EEPROM_WRITE_BASE, value);
      return;
   }
   
   if (address >= MATHBOX_WRITE_BASE && address <= MATHBOX_WRITE_END)
   {
      mathBox.Write(address - MATHBOX_WRITE_BASE, value);
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
         vectorGenerator.Go();
         break;
         
      case 0x5000:
         // watchdog timer clear... do nothing
         break;
         
      case 0x5800:
         // vector state machine reset
         vectorGenerator.Reset();
         break;
         
      case 0x6040:
         // doc says mathbox status, readonly, but the code writes to it
         break;
         
      case 0x60E0:
         // register for flashing the start buttons
         break;
         
      default:
         char  buffer[200];
         sprintf(buffer, "Invalid write address: 0x%X", address);
         throw TempestException(buffer);
   }
}
