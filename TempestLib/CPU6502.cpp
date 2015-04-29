
#include <stdio.h>

#include "AbstractBus.h"
#include "TempestException.h"

#include "CPU6502.h"

static const uint16_t NMI_VECTOR_ADDRESS = 0xFFFA;
static const uint16_t RESET_VECTOR_ADDRESS = 0xFFFC;
static const uint16_t IRQ_VECTOR_ADDRESS = 0xFFFE;
static const uint16_t BRK_VECTOR_ADDRESS = 0xFFFE;

CPU6502::CPU6502(AbstractBus *_bus)
{
   // save parameters
   bus = _bus;
}

void CPU6502::Reset(void)
{
   // start cold boot
   P.I = true;
   PC = GetResetVector();
}


uint16_t CPU6502::GetResetVector(void) const
{
   return GetU16At(RESET_VECTOR_ADDRESS);
}

uint16_t CPU6502::GetU16At(uint16_t address) const
{
   // low byte first
   return bus->ReadByte(address) + 256 * bus->ReadByte(address + 1);
}

void CPU6502::Run(void)
{
   for (;;)
   {
      // clear the current instruction log entry
      currentInstruction = InstructionLogEntry();
      
      // execute an instruction
      currentInstruction.ClockCycles += SingleStep();
      
      // log
      log.LogInstruction(currentInstruction);
   }
}

int CPU6502::SingleStep(void)
{
   // load the instruction
   currentInstruction.PC = PC;
   uint8_t opCode = bus->ReadByte(PC++);
   currentInstruction.OpCode = opCode;
   
   // process the instruction
   switch (opCode)
   {
      case 0x05: ORA(bus->ReadByte(PC++)); return 2;
      case 0x06: ASL(bus->ReadByte(PC++)); return 5;
      case 0x08: Push(P.ToByte()); return 3; //PHP
      case 0x09: ORA(PC++); return 2;
      case 0x0A: ASL(); return 2;
      case 0x10: BPL(); return 2;
      case 0x18: P.C = false; return 2; //CLC
      case 0x20: JSR(GetAbsoluteAddress()); return 6;
      case 0x26: ROL(bus->ReadByte(PC++)); return 5;
      case 0x28: P.FromByte(Pull()); return 4; //PLP
      case 0x29: AND(PC++); return 2;
      case 0x2A: ROL(); return 2;
      case 0x2C: BIT(GetAbsoluteAddress()); return 4;
      case 0x2D: AND(GetAbsoluteAddress()); return 4;
      case 0x30: BMI(); return 2;
      case 0x38: P.C = true; return 2; //SEC
      case 0x45: EOR(bus->ReadByte(PC++)); return 3;
      case 0x48: Push(A); return 3; //PHA
      case 0x49: EOR(PC++); return 2;
      case 0x4A: LSR(); return 2;
      case 0x4C: PC = GetAbsoluteAddress(); return 3; //JMP
      case 0x4D: EOR(GetAbsoluteAddress()); return 4;
      case 0x50: BVC(); return 2;
      case 0x51: EOR(GetIndirectYAddress()); return 6;
      case 0x59: EOR(GetAbsoluteAddress() + Y); return 5;
      case 0x60: RTS(); return 6;
      case 0x65: ADC(bus->ReadByte(PC++)); return 3;
      case 0x69: ADC(PC++); return 2;
      case 0x6A: ROR(); return 2;
      case 0x6D: ADC(GetAbsoluteAddress()); return 4;
      case 0x6E: ROR(GetAbsoluteAddress()); return 6;
      case 0x78: P.I = true; return 2; //SEI
      case 0x84: STY(bus->ReadByte(PC++)); return 3;
      case 0x85: STA(bus->ReadByte(PC++)); return 3;
      case 0x86: STX(bus->ReadByte(PC++)); return 3;
      case 0x88: DEY(); return 2;
      case 0x8A: A = X; SetNZ(A); return 2; //TXA
      case 0x8C: STY(GetAbsoluteAddress()); return 4;
      case 0x8D: STA(GetAbsoluteAddress()); return 4;
      case 0x8E: STX(GetAbsoluteAddress()); return 4;
      case 0x90: BCC(); return 2;
      case 0x91: STA(GetIndirectYAddress()); return 6;
      case 0x95: STA((uint16_t)bus->ReadByte(PC++) + X); return 4;
      case 0x96: STX((uint16_t)bus->ReadByte(PC++) + Y); return 4;
      case 0x98: A = Y; SetNZ(X); return 2; //TYA
      case 0x99: STA(GetAbsoluteAddress() + Y); return 5;
      case 0x9A: S = X; SetNZ(S); return 2; //TXS
      case 0x9D: STA(GetAbsoluteAddress() + X); return 5;
      case 0xA0: LDY(PC++); return 2;
      case 0xA2: LDX(PC++); return 2;
      case 0xA4: LDY(bus->ReadByte(PC++)); return 3;
      case 0xA5: LDA(bus->ReadByte(PC++)); return 3;
      case 0xA6: LDX(bus->ReadByte(PC++)); return 3;
      case 0xA8: Y = A; SetNZ(Y); return 2; //TAY
      case 0xA9: LDA(PC++); return 2;
      case 0xAA: X = A; SetNZ(X); return 2; //TAX
      case 0xAC: LDY(GetAbsoluteAddress()); return 4;
      case 0xAD: LDA(GetAbsoluteAddress()); return 4;
      case 0xAE: LDX(GetAbsoluteAddress()); return 4;
      case 0xB0: BCS(); return 2;
      case 0xB1: LDA(GetIndirectYAddress()); return 6;
      case 0xB5: LDA(bus->ReadByte(PC++) + X); return 4;
      case 0xB8: P.V = false; return 2; //CLV
      case 0xB9: LDA(GetAbsoluteAddress() + Y); return 4;
      case 0xBA: X = S; SetNZ(X); return 2; //TSX
      case 0xBC: LDY(GetAbsoluteAddress() + X); return 4;
      case 0xBD: LDA(GetAbsoluteAddress() + X); return 4;
      case 0xBE: LDX(GetAbsoluteAddress() + Y); return 4;
      case 0xC6: DEC(bus->ReadByte(PC++)); return 5;
      case 0xC8: INY(); return 2;
      case 0xC9: CMP(PC++); return 2;
      case 0xCA: SetNZ(--X); return 2; //DEX
      case 0xCD: CMP(GetAbsoluteAddress()); return 4;
      case 0xD0: BNE(); return 2;
      case 0xD1: CMP(GetIndirectYAddress()); return 6;
      case 0xD8: P.D = false; return 2; //CLD
      case 0xE0: CPX(PC++); return 2;
      case 0xE6: INC(bus->ReadByte(PC++)); return 5;
      case 0xE8: SetNZ(++X); return 2; //INX
      case 0xEA: return 2; //NOP
      case 0xEC: CPX(GetAbsoluteAddress()); return 4;
      case 0xEE: INC(GetAbsoluteAddress()); return 6;
      case 0xF0: BEQ(); return 2;
      
      default:
      {
         char  buffer[100];
         sprintf(buffer, "Unimplemented op code: %X", opCode);
         throw TempestException(buffer);
      }
   }
}


void CPU6502::Compare(uint8_t a, uint8_t b)
{
   SetNZ(a - b);
   P.C = a >= b;
}

uint8_t CPU6502::DoASL(uint8_t value)
{
   P.C = (value & 0x80) != 0;
   uint8_t result = value << 1;
   SetNZ(result);
   return result;
}

uint8_t CPU6502::DoLSR(uint8_t value)
{
   P.C = (value & 0x01) != 0;
   uint8_t result = value >> 1;
   SetNZ(result);
   return result;
}

uint8_t CPU6502::DoROL(uint8_t value)
{
   bool carry = P.C;
   P.C = (value & 0x80) != 0;
   value <<= 1;
   if (carry)
      value |= 0x01;
   SetNZ(value);
   return value;
}

uint8_t CPU6502::DoROR(uint8_t value)
{
   bool carry = P.C;
   P.C = (value & 1) != 0;
   value >>= 1;
   if (carry)
      value |= 0x80;
   SetNZ(value);
   return value;
}

uint16_t CPU6502::GetAbsoluteAddress(void)
{
   return bus->ReadByte(PC++) + 256 * bus->ReadByte(PC++);
}

uint16_t CPU6502::GetIndirectYAddress(void)
{
   uint16_t zp = bus->ReadByte(PC++);
   return bus->ReadByte(zp) + 256 * bus->ReadByte(zp + 1) + Y;
}

uint8_t CPU6502::Pull(void)
{
   return bus->ReadByte(0x100 + ++S);
}

void CPU6502::Push(uint8_t value)
{
   bus->WriteByte(0x100 + S--, value);
}


void CPU6502::SetNZ(uint8_t value)
{
   P.N = (value & 0x80) != 0;
   P.Z = value == 0;
}




void CPU6502::ADC(uint16_t address)
{
   if (P.D)
      throw TempestException("ADC: decimal mode not supported");
   
   uint8_t value = bus->ReadByte(address);
   int unsignedResult = A + value + (P.C?1:0);
   int signedResult = (int8_t)A + (int8_t)value + (P.C?1:0);
   
   A = (uint8_t)unsignedResult;
   P.V = signedResult>127 || signedResult<-128;
   P.C = unsignedResult > 255;
   SetNZ(A);
}

void CPU6502::AND(uint16_t address)
{
   A &= bus->ReadByte(address);
   SetNZ(A);
}

void CPU6502::ASL(void)
{
   A = DoASL(A);
}

void CPU6502::ASL(uint16_t address)
{
   bus->WriteByte(address, DoASL(bus->ReadByte(address)));
}

void CPU6502::BCC(void)
{
   int branchDistance = (int8_t)bus->ReadByte(PC++);
   if (!P.C)
      PC += branchDistance;
}

void CPU6502::BCS(void)
{
   int branchDistance = (int8_t)bus->ReadByte(PC++);
   if (P.C)
      PC += branchDistance;
}

void CPU6502::BEQ(void)
{
   int branchDistance = (int8_t)bus->ReadByte(PC++);
   if (P.Z)
      PC += branchDistance;
}

void CPU6502::BIT(uint16_t address)
{
   currentInstruction.Mnemonic = OP_BIT;
   uint8_t memoryValue = bus->ReadByte(address);
   P.Z = (A & memoryValue) == 0;
   P.N = (memoryValue & 0x80) != 0;
   P.V = (memoryValue & 0x40) != 0;
}

void CPU6502::BMI(void)
{
   currentInstruction.Mnemonic = OP_BMI;
   int branchDistance = (int8_t)bus->ReadByte(PC++);
   if (P.N)
      PC += branchDistance;
}

void CPU6502::BNE(void)
{
   currentInstruction.Mnemonic = OP_BNE;
   int branchDistance = (int8_t)bus->ReadByte(PC++);
   if (!P.Z)
      PC += branchDistance;
}

void CPU6502::BPL(void)
{
   this->currentInstruction.Mnemonic = OP_BPL;
   int branchDistance = (int8_t)bus->ReadByte(PC++);
   if (!P.N)
      PC += branchDistance;
}

void CPU6502::BVC(void)
{
   this->currentInstruction.Mnemonic = OP_BVC;
   int branchDistance = (int8_t)bus->ReadByte(PC++);
   if (!P.V)
      PC += branchDistance;
}

void CPU6502::CMP(uint16_t address)
{
   Compare(A, bus->ReadByte(address));
}

void CPU6502::CPX(uint16_t address)
{
   Compare(X, bus->ReadByte(address));
}

void CPU6502::DEC(uint16_t address)
{
   uint8_t result = bus->ReadByte(address) - 1;
   bus->WriteByte(address, result);
   SetNZ(result);
}

void CPU6502::DEY(void)
{
   this->currentInstruction.Mnemonic = OP_DEY;
   SetNZ(--Y);
}

void CPU6502::EOR(uint16_t address)
{
   currentInstruction.Mnemonic = OP_EOR;
   A ^= bus->ReadByte(address);
   SetNZ(A);
}

void CPU6502::INC(uint16_t address)
{
   currentInstruction.Mnemonic = OP_INC;
   uint8_t result = bus->ReadByte(address) + 1;
   bus->WriteByte(address, result);
   SetNZ(result);
}

void CPU6502::INY(void)
{
   this->currentInstruction.Mnemonic = OP_INY;
   SetNZ(++Y);
}


void CPU6502::JSR(uint16_t address)
{
   uint16_t returnAddress = PC - 1;
   Push((uint8_t)(returnAddress>>8));
   Push((uint8_t)returnAddress);
   PC = address;
}

void CPU6502::LDA(uint16_t address)
{
   A = bus->ReadByte(address);
   SetNZ(A);
}

void CPU6502::LDX(uint16_t address)
{
   X = bus->ReadByte(address);
   SetNZ(X);
}

void CPU6502::LDY(uint16_t address)
{
   Y = bus->ReadByte(address);
   SetNZ(Y);
}

void CPU6502::LSR(void)
{
   A = DoLSR(A);
}

void CPU6502::ORA(uint16_t address)
{
   A |= bus->ReadByte(address);
   SetNZ(A);
}

void CPU6502::ROL(void)
{
   A = DoROL(A);
}

void CPU6502::ROL(uint16_t address)
{
   bus->WriteByte(address, DoROL(bus->ReadByte(address)));
}

void CPU6502::ROR(void)
{
   A = DoROR(A);
}

void CPU6502::ROR(uint16_t address)
{
   bus->WriteByte(address, DoROR(bus->ReadByte(address)));
}

void CPU6502::RTS(void)
{
   PC = Pull() + 256*Pull() + 1;
}

void CPU6502::STA(uint16_t address)
{
   currentInstruction.Mnemonic = OP_STA;
   bus->WriteByte(address, A);
}

void CPU6502::STX(uint16_t address)
{
   bus->WriteByte(address, X);
}

void CPU6502::STY(uint16_t address)
{
   bus->WriteByte(address, Y);
}

