// ====================================================================
// 6502 emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is the instruction processor of the 6502... it interacts
//    with an AbstractBus object, which supplies everything that the
//    6502 connects to.
// ====================================================================

#ifndef CPU6502_H
#define	CPU6502_H

#include "ProcessorStatusWord.h"

// === hardcoded processor memory locations
const uint16_t NMI_VECTOR_ADDRESS = 0xFFFA;
const uint16_t RESET_VECTOR_ADDRESS = 0xFFFC;
const uint16_t IRQ_VECTOR_ADDRESS = 0xFFFE;
const uint16_t BRK_VECTOR_ADDRESS = 0xFFFE;

class AbstractBus;

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

class CPU6502
{
public:
   CPU6502(AbstractBus *_bus);

   void     Reset(void);
   void     Run(void);
   uint8_t  SingleStep(void);

	// simple accessors
	uint8_t  GetA(void) const { return A; }
	uint16_t GetPC(void) const { return PC; }
	uint8_t  GetX(void) const { return X; }
	uint8_t  GetY(void) const { return Y; }
	uint8_t  GetS(void) const { return S; }

   // ops... not all are implemented in a way that makes sense to make them
   // public... I'm publicizing as I need them as to not let things get crazy
   void  JMP(uint16_t address) { PC = address; }
   void  RTS(void);

   // utility
   static uint8_t FromBCD(uint8_t value);
   static uint8_t ToBCD(uint8_t value);

private:
   void		EnterISR(uint16_t interruptVectorAddress);
   void     Compare(uint8_t a, uint8_t b);
   uint8_t  DoASL(uint8_t value);
   uint8_t  DoLSR(uint8_t value);
   uint8_t  DoROL(uint8_t value);
   uint8_t  DoROR(uint8_t value);
   uint16_t GetAbsoluteAddress(void);
   uint16_t GetIndirectXAddress(void);
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
   void  BRK(void);
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
	void  LSR(uint16_t address);
	void  ORA(uint16_t address);
   void  ROL(void);
   void  ROL(uint16_t address);
   void  ROR(void);
   void  ROR(uint16_t address);
	void  RTI(void);
	void  SBC(uint16_t address);
	void  STA(uint16_t address);
	void  STX(uint16_t address);
   void  STY(uint16_t address);

private:
   AbstractBus *bus = nullptr;

   uint8_t  A;
   uint8_t  S;
   uint8_t  X;
   uint8_t  Y;
   ProcessorStatusWord  P;
   uint16_t PC;

   bool inNMI = false;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif	/* CPU6502_H */

