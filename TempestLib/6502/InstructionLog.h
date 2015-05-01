/* 
 * File:   InstructionLog.h
 * Author: Randy
 *
 * Created on April 26, 2015, 7:17 AM
 */

#ifndef INSTRUCTIONLOG_H
#define	INSTRUCTIONLOG_H

#include <vector>
#include "InstructionLogEntry.h"

class InstructionLog
{
public:
   InstructionLog();
   
	int  GetTotalClockCycles(void) { return totalClockCycles; }
	void LogInstruction(const InstructionLogEntry &_entry);
   void Print(void);
   
private:    
   int instructionsExecuted;
   int totalClockCycles;
   int logStart;
   int logCount;
   std::vector<InstructionLogEntry> logEntries;
};

#endif	/* INSTRUCTIONLOG_H */

