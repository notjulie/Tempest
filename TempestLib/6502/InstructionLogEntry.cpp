
#include <stdio.h>
#include "InstructionLogEntry.h"

InstructionLogEntry::InstructionLogEntry(void)
{
   ClockCycles = 0;
   Mnemonic = OP_UNK;
   PC = 0;
   OpCode = 0;
}

void InstructionLogEntry::Print(void) const
{
   printf("%4X %s %02X\n", PC, GetMnemonicString(Mnemonic).c_str(), OpCode);
}
