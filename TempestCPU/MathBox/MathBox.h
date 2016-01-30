/* 
 * File:   MathBox.h
 * Author: Randy
 *
 * Created on April 27, 2015, 4:22 PM
 */

#ifndef MATHBOX_H
#define	MATHBOX_H

#include "Am2901.h"

class MathBoxLog;

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

class MathBox
{
private:
	enum Bit {
		BIT_COUNT
	};

public:
	MathBox(void);
	virtual ~MathBox(void);

   uint8_t GetStatus(void);
   uint8_t ReadLow(void);
   uint8_t ReadHigh(void);
   void Write(uint8_t address, uint8_t value);

private:
	bool GetQ0(void);
	void SetALUCarryFlags(void);
	void SetALUInputs(void);

private:
	void HandleRisingClock(void);
	void HandleFallingClock(void);

private:
	// forbidden items
	MathBox(const MathBox &mathBox);
	MathBox &operator=(const MathBox &mathBox);

private:
	// inputs
	uint8_t dataIn;
	uint8_t addressIn;
	bool BEGIN;

	// state values that change on rising clock
	uint8_t   PC;
	bool  Q0Latch;

	// state values that change on falling clock
	bool STOP;
	uint8_t JumpLatch;

	// ALUs
	Am2901	aluK;
	Am2901	aluF;
	Am2901	aluJ;
	Am2901	aluE;

	// ROMs
	const uint8_t *romA;
	const uint8_t *romE;
	const uint8_t *romF;
	const uint8_t *romH;
	const uint8_t *romJ;
	const uint8_t *romK;
	const uint8_t *romL;

	friend class MathBoxTestInterface;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif	/* MATHBOX_H */

