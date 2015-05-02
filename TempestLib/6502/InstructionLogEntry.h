/* 
 * File:   InstructionLogEntry.h
 * Author: Randy
 *
 * Created on April 26, 2015, 7:19 AM
 */

#ifndef INSTRUCTIONLOGENTRY_H
#define	INSTRUCTIONLOGENTRY_H

#include "Mnemonic6502.h"

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class InstructionLogEntry
{
public:
   InstructionLogEntry();
   void Print(void) const;
   
public:
   int   ClockCycles;
   uint16_t PC;
   Mnemonic6502 Mnemonic;
   uint8_t OpCode;
};

#pragma warning(pop)

#endif	/* INSTRUCTIONLOGENTRY_H */

