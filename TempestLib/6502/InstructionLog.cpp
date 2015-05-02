
#include "stdafx.h"

#include "CPU6502Exception.h"

#include "InstructionLog.h"


InstructionLog::InstructionLog(void)
{
   instructionsExecuted = 0;
   totalClockCycles = 0;
   logEntries.resize(100);
   
   logStart = 0;
   logCount = 0;
}

void InstructionLog::LogInstruction(const InstructionLogEntry &_entry)
{
   // update our stats
   totalClockCycles += _entry.ClockCycles;
   ++instructionsExecuted;
   
   // add the entry
   unsigned index = (logStart + logCount) % logEntries.size();
   logEntries[index] = _entry;
   if (logCount == (int)logEntries.size())
      ++logStart;
   else
      ++logCount;
}


void InstructionLog::Print(void)
{
   printf("Instructions executed: %d\n", instructionsExecuted);
   printf("Total clock cycles: %d\n", totalClockCycles);
   
   for (int i=0; i<logCount; ++i)
   {
      unsigned index = (logStart + i) % logEntries.size();
      logEntries[index].Print();
   }
}
