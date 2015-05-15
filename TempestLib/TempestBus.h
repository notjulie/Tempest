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

#include "AbstractTempestWaveStream.h"
#include "EEPROM.h"
#include "TempestPokey.h"

class VectorData;

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures


class TempestBus : public AbstractBus, public AbstractTempestWaveStream
{
public:
	TempestBus(AbstractTempestEnvironment *_environment);
	virtual ~TempestBus(void);

	void LoadROM(const uint8_t *rom, int length, uint16_t address);
	void LoadMathBoxROM(const uint8_t *rom, int length, char slot);

	std::string GetMathBoxStatus(void) const { return std::string(); }
	
   bool HaveNewVectorData(void);
	void PopVectorData(VectorData &_vectorData);

	void MoveWheel(int delta) { pokey1.MoveWheel(delta); }
	void SetButtonState(ButtonID button, bool pressed) { pokey2.SetButtonState(button, pressed); }
	void Set3KHzClockState(bool state) { clock3KHzIsHigh = state; }

public:
	// AbstractBus overrides
	virtual uint8_t ReadByte(uint16_t address);
   virtual void    WriteByte(uint16_t address, uint8_t value);

public:
	// AbstractTempestWaveStream overrides
	virtual void ReadWaveData(int16_t *buffer, int count);

private:
	// forbidden items
	TempestBus(const TempestBus &bus);
	TempestBus &operator=(const TempestBus &bus);

private:
   std::vector<uint8_t>  rom;
   std::vector<uint8_t>  mainRAM;
   std::vector<uint8_t>  colorRAM;
   TempestPokey1 pokey1;
   TempestPokey2 pokey2;
   EEPROM eeprom;
   MathBox mathBox;
   VectorGenerator vectorGenerator;

	bool selfTest;
	bool slam;
	bool clock3KHzIsHigh;
};

#pragma warning(pop)

#endif	/* TEMPESTBUS_H */

