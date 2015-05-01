/* 
 * File:   MathBox.h
 * Author: Randy
 *
 * Created on April 27, 2015, 4:22 PM
 */

#ifndef MATHBOX_H
#define	MATHBOX_H

#include <string>
#include <vector>

#include "Am2901.h"

class MathBox
{
private:
	enum Bit {
		PCEN
	};

public:
	void LoadROM(const uint8_t *rom, int length, char slot);

	std::string GetErrorString(void) const { return error.size() == 0 ? "OK" : error; }
   uint8_t GetStatus(void);
   uint8_t Read1(void);
   uint8_t Read2(void);
   void Write(uint8_t address, uint8_t value);

private:
	bool GetBit(Bit bit);
	void SetError(const std::string &_status);

private:
	void HandleRisingClock(void);
	void HandleFallingClock(void);

private:
	// inputs
	int dataIn;
	int addressIn;
	bool BEGIN;

	// state values that change on rising clock
	int   PC;

	// state values that change on falling clock
	bool STOP;

	// ALUs
	Am2901	aluK;
	Am2901	aluF;
	Am2901	aluJ;
	Am2901	aluE;

	// ROMs
	std::vector<uint8_t> romA;
	std::vector<uint8_t> romE;
	std::vector<uint8_t> romF;
	std::vector<uint8_t> romH;
	std::vector<uint8_t> romJ;
	std::vector<uint8_t> romK;
	std::vector<uint8_t> romL;

	// misc
	std::string	error;
};

#endif	/* MATHBOX_H */

