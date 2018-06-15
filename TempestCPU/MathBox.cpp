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

#include "TempestCPU.Headers.h"
#include "MathBox.h"


/// <summary>
/// Constructor.  Boring.
/// </summary>
MathBox::MathBox(void)
{
}


/// <summary>
/// Destructor.  Boring.
/// </summary>
MathBox::~MathBox(void)
{
}


/// <summary>
/// The processor reads this through memory mapped I/O to determine if the MathBox
/// is idle, since the real MathBox runs asynchronously.  Since we do everything
/// synchronously on the thread that calls us, we are always idle.  Therefore,
/// boring.
/// </summary>
uint8_t MathBox::GetStatus(void)
{
	return 0;
}

/// <summary>
/// Whenever the CPU writes to us, the address tells us an operation to carry out,
/// and the value may or may not be of any importance.  For example, writing to
/// address 0x14 simply means "calculate based on previous inputs".  If I wanted
/// to fully emulate the MathBox, I would need to account for things like that.
/// As it is, I am just emulating the MathBox to deal with the way Tempest uses it.
/// As such, we just need to record whatever is last written to each address.
/// </summary>
void MathBox::Write(uint8_t address, uint8_t value)
{
   inputs[address] = value;
}


/// <summary>
/// Having reverse engineered the operation of the MathBox, it turns out all
/// that it is really used for is a single operation: floating point (sorta)
/// division.  The inputs are:
///    address 0x0E: numerator base
///    address 0x1C: numerator exponent (2^x)
///    address 0x16: denominator high byte
///    address 0x15: denominator low byte
/// So this is all it ends up being.  Oh, except that the exponent is 8 plus
/// the actual exponent, so we end up with an extra factor of 256 at the end.
/// </summary>
uint16_t MathBox::CalculateResult(void) const
{
   int64_t result =
      (1 << inputs[0x0C]) *
      inputs[0x0E] /
      (256 * inputs[0x16] + inputs[0x15]);
   return (uint16_t)(result >> 8);
}


/// <summary>
/// Returns the low byte of the current result.
/// </summary>
uint8_t MathBox::ReadLow(void)
{
   return (uint8_t)CalculateResult();
}


/// <summary>
/// Returns the high byte of the current result.
/// </summary>
uint8_t MathBox::ReadHigh(void)
{
   return (uint8_t)(CalculateResult() >> 8);
}

