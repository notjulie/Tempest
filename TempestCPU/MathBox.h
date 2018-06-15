// ====================================================================
// Tempest emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    Emulation of the Tempest MathBox circuit.  Originally this was
//    a full hardware emulation of all the hardware gates, ROMs,
//    counters, latches and Am2901 ALU chips.  Once I got that working
//    I left it for the longest time, knowing that it was using a
//    whole lot of CPU for what I figured was a fairly simple
//    operation.  Eventually, I popped into the debugger, dumped
//    out some data, looked at it in LibreOffice and figure out what
//    it was doing.  This file was quite a bit more interesting until
//    I did that.  Take a look at its history on GitHub if you're
//    interested.
// ====================================================================

#ifndef MATHBOX_H
#define	MATHBOX_H

#include <stdint.h>

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif


/// <summary>
/// Mathbox emulator
/// </summary>
class MathBox
{
public:
	MathBox(void);
	~MathBox(void);

   uint8_t GetStatus(void);
   uint8_t ReadLow(void);
   uint8_t ReadHigh(void);
   void Write(uint8_t address, uint8_t value);

private:
   uint16_t CalculateResult(void) const;

private:
   uint8_t inputs[32];
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif	/* MATHBOX_H */

