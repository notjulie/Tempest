/* 
 * File:   TempestBus.h
 * Author: Randy
 *
 * Created on April 25, 2015, 1:50 PM
 */

#ifndef TEMPESTBUS_H
#define	TEMPESTBUS_H

#include "6502/AbstractBus.h"
#include "MathBox/MathBox.h"
#include "Vector/VectorGenerator.h"

#include "EEPROM.h"
#include "Pokey.h"


class Abstract3KHzClock;
class VectorData;

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class TempestBus : public AbstractBus
{
public:
	TempestBus(Abstract3KHzClock *_clock3KHz);
	virtual ~TempestBus(void);

	void LoadROM(const uint8_t *rom, int length, uint16_t address);
	void LoadMathBoxROM(const uint8_t *rom, int length, char slot);

	std::string GetMathBoxStatus(void) const { return mathBox.GetErrorString(); }
	std::string GetMathBoxLogXML(void) const { return mathBox.GetLogXML(); }
	
   bool HaveNewVectorData(void);
	void PopVectorData(VectorData &_vectorData);

public:
   virtual uint8_t ReadByte(uint16_t address);   
   virtual void    WriteByte(uint16_t address, uint8_t value);

private:
	// forbidden items
	TempestBus(const TempestBus &bus);
	TempestBus &operator=(const TempestBus &bus);

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

#pragma warning(pop)

#endif	/* TEMPESTBUS_H */

