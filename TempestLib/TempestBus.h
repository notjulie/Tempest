/* 
 * File:   TempestBus.h
 * Author: Randy
 *
 * Created on April 25, 2015, 1:50 PM
 */

#ifndef TEMPESTBUS_H
#define	TEMPESTBUS_H

#include <vector>

#include "6502/AbstractBus.h"
#include "EEPROM.h"
#include "MathBox.h"
#include "Pokey.h"
#include "VectorGenerator.h"


class Abstract3KHzClock;

class TempestBus : public AbstractBus
{
public:
   TempestBus(Abstract3KHzClock *_clock3KHz);
   
   void LoadROM(const std::string &_path, uint16_t address);
   void LoadROM(const uint8_t *rom, int length, uint16_t address);

public:
   virtual uint8_t ReadByte(uint16_t address);   
   virtual void    WriteByte(uint16_t address, uint8_t value);
   
private:
   Abstract3KHzClock *clock3KHz;
   
   std::vector<uint8_t>  rom;
   std::vector<uint8_t>  mainRAM;
   std::vector<uint8_t>  colorRAM;
   Pokey pokey1;
   Pokey pokey2;
   EEPROM eeprom;
   MathBox mathBox;
   VectorGenerator vectorGenerator;
};

#endif	/* TEMPESTBUS_H */

