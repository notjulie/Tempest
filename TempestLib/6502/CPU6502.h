/* 
 * File:   CPU6502.h
 * Author: Randy
 *
 * Created on April 25, 2015, 1:51 PM
 */

#ifndef CPU6502_H
#define	CPU6502_H

#include "ProcessorStatusWord.h"

class AbstractBus;


#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class CPU6502
{
public:
   CPU6502(AbstractBus *_bus);

	uint64_t GetTotalClockCycles(void) { return totalClockCycles; }
	uint64_t GetIRQCount(void) { return irqsHandled; }

   void     Reset(void);
   void     Run(void);
   void		SingleStep(void);

private:
   int  DoSingleStep(void);

private:
   void     Compare(uint8_t a, uint8_t b);
   uint8_t  DoASL(uint8_t value);
   uint8_t  DoLSR(uint8_t value);
   uint8_t  DoROL(uint8_t value);
   uint8_t  DoROR(uint8_t value);
   uint16_t GetAbsoluteAddress(void);
   uint16_t GetIndirectYAddress(void);
   uint16_t GetResetVector(void) const;
   uint16_t GetU16At(uint16_t address) const;
   uint8_t  Pull(void);
   void     Push(uint8_t value);
   void     SetNZ(uint8_t value);
   
private:
   void  ADC(uint16_t address);
   void  AND(uint16_t address);
   void  ASL(void);
   void  ASL(uint16_t address);
   void  BCC(void);
   void  BCS(void);
   void  BEQ(void);
   void  BIT(uint16_t address);
   void  BMI(void);
   void  BNE(void);
   void  BPL(void);
	void  BVC(void);
	void  BVS(void);
	void  CMP(uint16_t address);
	void  CPX(uint16_t address);
	void  CPY(uint16_t address);
	void  DEC(uint16_t address);
   void  DEY(void);
   void  EOR(uint16_t address);
   void  INC(uint16_t address);
   void  INY(void);
   void  JSR(uint16_t address);
   void  LDA(uint16_t address);
   void  LDX(uint16_t address);
   void  LDY(uint16_t address);
   void  LSR(void);
   void  ORA(uint16_t address);
   void  ROL(void);
   void  ROL(uint16_t address);
   void  ROR(void);
   void  ROR(uint16_t address);
	void  RTI(void);
	void  RTS(void);
	void  SBC(uint16_t address);
	void  STA(uint16_t address);
	void  STX(uint16_t address);
   void  STY(uint16_t address);
   
private:
	static uint8_t FromBCD(uint8_t value);
	static uint8_t ToBCD(uint8_t value);

private:
   AbstractBus *bus;
	uint64_t irqsHandled;
	uint64_t totalClockCycles;

   uint8_t  A;
   uint8_t  S;
   uint8_t  X;
   uint8_t  Y;
   ProcessorStatusWord  P;
   uint16_t PC;
};

#pragma warning(pop)

#endif	/* CPU6502_H */

