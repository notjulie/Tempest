
#include <stdio.h>
#include <vector>

#include "TempestException.h"

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
   
   // for the moment
   if (instructionsExecuted > 10000000)
      throw TempestException("Instruction limit hit");
   
   // add the entry
   int index = (logStart + logCount) % logEntries.size();
   logEntries[index] = _entry;
   if (logCount == logEntries.size())
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
      int index = (logStart + i) % logEntries.size();
      logEntries[index].Print();
   }
}
