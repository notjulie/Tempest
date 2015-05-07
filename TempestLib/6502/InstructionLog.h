/* 
 * File:   InstructionLog.h
 * Author: Randy
 *
 * Created on April 26, 2015, 7:17 AM
 */

#ifndef INSTRUCTIONLOG_H
#define	INSTRUCTIONLOG_H

#include "InstructionLogEntry.h"

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class InstructionLog
{
public:
   InstructionLog();
   
	uint64_t  GetTotalClockCycles(void) { return totalClockCycles; }
	void LogInstruction(const InstructionLogEntry &_entry);
   void Print(void);
   
private:    
   int instructionsExecuted;
   uint64_t totalClockCycles;
   int logStart;
   int logCount;
   std::vector<InstructionLogEntry> logEntries;
};

#pragma warning(pop)

#endif	/* INSTRUCTIONLOG_H */

