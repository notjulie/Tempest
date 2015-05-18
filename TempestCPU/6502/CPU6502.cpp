
#include "stdafx.h"
#include "AbstractBus.h"
#include "CPU6502Exception.h"

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

void CPU6502::IRQ(void)
{
	// ignore if interrupts are disabled
	if (P.I)
		return;

	// push the current PC
	Push((uint8_t)(PC >> 8));
	Push((uint8_t)PC);

	// push the processor status register
	Push(P.ToByte());

	// disable interrupts
	P.I = true;

	// jump to the ISR
	PC = GetU16At(IRQ_VECTOR_ADDRESS);
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
	return (uint16_t)(bus->ReadByte(address) + 256 * bus->ReadByte((uint16_t)(address + 1)));
}

void CPU6502::Run(void)
{
   for (;;)
	   SingleStep();
}

int CPU6502::SingleStep(void)
{
   // load the instruction
	uint16_t instructionAddress = PC;
	uint8_t opCode = bus->ReadByte(PC++);
   
   // process the instruction
   switch (opCode)
   {
      case 0x05: ORA(bus->ReadByte(PC++)); return 2;
      case 0x06: ASL(bus->ReadByte(PC++)); return 5;
      case 0x08: Push(P.ToByte()); return 3; //PHP
      case 0x09: ORA(PC++); return 2;
      case 0x0A: ASL(); return 2;
		case 0x0D: ORA(GetAbsoluteAddress()); return 4;
		case 0x10: BPL(); return 2;
      case 0x18: P.C = false; return 2; //CLC
		case 0x1D: ORA((uint16_t)(GetAbsoluteAddress() + X)); return 4;
		case 0x1E: ASL((uint16_t)(GetAbsoluteAddress() + X)); return 7;
		case 0x20: JSR(GetAbsoluteAddress()); return 6;
		case 0x24: BIT(bus->ReadByte(PC++)); return 3;
		case 0x25: AND(bus->ReadByte(PC++)); return 2;
		case 0x26: ROL(bus->ReadByte(PC++)); return 5;
		case 0x28: P.FromByte(Pull()); return 4; //PLP
      case 0x29: AND(PC++); return 2;
      case 0x2A: ROL(); return 2;
      case 0x2C: BIT(GetAbsoluteAddress()); return 4;
      case 0x2D: AND(GetAbsoluteAddress()); return 4;
      case 0x30: BMI(); return 2;
      case 0x38: P.C = true; return 2; //SEC
		case 0x39: AND((uint16_t)(GetAbsoluteAddress() + Y)); return 4;
		case 0x40: RTI(); return 6;
      case 0x45: EOR(bus->ReadByte(PC++)); return 3;
      case 0x48: Push(A); return 3; //PHA
      case 0x49: EOR(PC++); return 2;
      case 0x4A: LSR(); return 2;
      case 0x4C: PC = GetAbsoluteAddress(); return 3; //JMP
      case 0x4D: EOR(GetAbsoluteAddress()); return 4;
		case 0x4E: LSR(GetAbsoluteAddress()); return 6;
		case 0x50: BVC(); return 2;
      case 0x51: EOR(GetIndirectYAddress()); return 6;
		case 0x55: EOR((uint16_t)(bus->ReadByte(PC++) + X)); return 4;
		case 0x58: P.I = false; return 2; // CLI
      case 0x59: EOR((uint16_t)(GetAbsoluteAddress() + Y)); return 5;
		case 0x5D: EOR((uint16_t)(GetAbsoluteAddress() + X)); return 4;
		case 0x60: RTS(); return 6;
      case 0x65: ADC(bus->ReadByte(PC++)); return 3;
		case 0x66: ROR(bus->ReadByte(PC++)); return 5;
		case 0x68: A = Pull(); SetNZ(A); return 4; //PLA
		case 0x69: ADC(PC++); return 2;
		case 0x6A: ROR(); return 2;
      case 0x6D: ADC(GetAbsoluteAddress()); return 4;
		case 0x6E: ROR(GetAbsoluteAddress()); return 6;
		case 0x70: BVS(); return 2;
		case 0x71: ADC(GetIndirectYAddress()); return 5;
		case 0x75: ADC((uint16_t)(bus->ReadByte(PC++) + X)); return 4;
		case 0x78: P.I = true; return 2; //SEI
		case 0x79: ADC((uint16_t)(GetAbsoluteAddress() + Y)); return 4;
		case 0x7D: ADC((uint16_t)(GetAbsoluteAddress() + X)); return 4;
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
		case 0x94: STY((uint16_t)(bus->ReadByte(PC++) + X)); return 4;
		case 0x95: STA((uint16_t)(bus->ReadByte(PC++) + X)); return 4;
      case 0x96: STX((uint16_t)(bus->ReadByte(PC++) + Y)); return 4;
      case 0x98: A = Y; SetNZ(A); return 2; //TYA
		case 0x99: STA((uint16_t)(GetAbsoluteAddress() + Y)); return 5;
      case 0x9A: S = X; SetNZ(S); return 2; //TXS
		case 0x9D: STA((uint16_t)(GetAbsoluteAddress() + X)); return 5;
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
		case 0xB4: LDY((uint16_t)(bus->ReadByte(PC++) + X)); return 4;
		case 0xB5: LDA((uint16_t)(bus->ReadByte(PC++) + X)); return 4;
		case 0xB8: P.SetOverflow(false); return 2; //CLV
		case 0xB9: LDA((uint16_t)(GetAbsoluteAddress() + Y)); return 4;
      case 0xBA: X = S; SetNZ(X); return 2; //TSX
		case 0xBC: LDY((uint16_t)(GetAbsoluteAddress() + X)); return 4;
		case 0xBD: LDA((uint16_t)(GetAbsoluteAddress() + X)); return 4;
		case 0xBE: LDX((uint16_t)(GetAbsoluteAddress() + Y)); return 4;
		case 0xC0: CPY(PC++); return 2;
		case 0xC4: CPY(bus->ReadByte(PC++)); return 3;
		case 0xC5: CMP(bus->ReadByte(PC++)); return 3;
		case 0xC6: DEC(bus->ReadByte(PC++)); return 5;
		case 0xC8: INY(); return 2;
      case 0xC9: CMP(PC++); return 2;
      case 0xCA: SetNZ(--X); return 2; //DEX
		case 0xCC: CPY(GetAbsoluteAddress()); return 4;
		case 0xCD: CMP(GetAbsoluteAddress()); return 4;
		case 0xCE: DEC(GetAbsoluteAddress()); return 6;
		case 0xD0: BNE(); return 2;
      case 0xD1: CMP(GetIndirectYAddress()); return 6;
		case 0xD6: DEC((uint16_t)(bus->ReadByte(PC++) + X)); return 6;
		case 0xD8: P.D = false; return 2; //CLD
		case 0xD9: CMP((uint16_t)(GetAbsoluteAddress() + Y)); return 4;
		case 0xDD: CMP((uint16_t)(GetAbsoluteAddress() + X)); return 4;
		case 0xDE: DEC((uint16_t)(GetAbsoluteAddress() + X)); return 7;
		case 0xE0: CPX(PC++); return 2;
		case 0xE4: CPX(bus->ReadByte(PC++)); return 3;
		case 0xE5: SBC(bus->ReadByte(PC++)); return 3;
		case 0xE6: INC(bus->ReadByte(PC++)); return 5;
		case 0xE8: SetNZ(++X); return 2; //INX
		case 0xE9: SBC(PC++); return 2;
		case 0xEA: return 2; //NOP
      case 0xEC: CPX(GetAbsoluteAddress()); return 4;
		case 0xED: SBC(GetAbsoluteAddress()); return 4;
		case 0xEE: INC(GetAbsoluteAddress()); return 6;
		case 0xF0: BEQ(); return 2;
		case 0xF1: SBC(GetIndirectYAddress()); return 5;
		case 0xF5: SBC((uint16_t)(bus->ReadByte(PC++) + X)); return 3;
		case 0xF6: INC((uint16_t)(bus->ReadByte(PC++) + X)); return 6;
		case 0xF8: P.D = true; return 2; //SED
		case 0xF9: SBC((uint16_t)(GetAbsoluteAddress() + Y)); return 4;
		case 0xFD: SBC((uint16_t)(GetAbsoluteAddress() + X)); return 4;
		case 0xFE: INC((uint16_t)(GetAbsoluteAddress() + X)); return 7;

      default:
      {
         char  buffer[100];
			sprintf_s(buffer, "Unimplemented op code at %04X: %02X", instructionAddress, opCode);
         throw CPU6502Exception(buffer);
      }
   }
}


void CPU6502::Compare(uint8_t a, uint8_t b)
{
   SetNZ((uint8_t)(a - b));
   P.C = a >= b;
}

uint8_t CPU6502::DoASL(uint8_t value)
{
   P.C = (value & 0x80) != 0;
	uint8_t result = (uint8_t)(value << 1);
   SetNZ(result);
   return result;
}

uint8_t CPU6502::DoLSR(uint8_t value)
{
   P.C = (value & 0x01) != 0;
	uint8_t result = (uint8_t)(value >> 1);
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
	return (uint16_t)(bus->ReadByte(PC++) + 256 * bus->ReadByte(PC++));
}

uint16_t CPU6502::GetIndirectYAddress(void)
{
   uint16_t zp = bus->ReadByte(PC++);
	return (uint16_t)(bus->ReadByte(zp) + 256 * bus->ReadByte((uint16_t)(zp + 1)) + Y);
}

uint8_t CPU6502::Pull(void)
{
	return bus->ReadByte((uint16_t)(0x100 + ++S));
}

void CPU6502::Push(uint8_t value)
{
	bus->WriteByte((uint16_t)(0x100 + S--), value);
}


void CPU6502::SetNZ(uint8_t value)
{
   P.N = (value & 0x80) != 0;
   P.Z = value == 0;
}




void CPU6502::ADC(uint16_t address)
{
	uint8_t value = bus->ReadByte(address);

	if (P.D)
	{
		uint8_t v = FromBCD(value);
		uint8_t a = FromBCD(A);
		int result = a + v + (P.C ? 1 : 0);
		if (result > 99)
		{
			result -= 100;
			P.C = true;
		}
		else
		{
			P.C = false;
		}

		A = ToBCD((uint8_t)result);

		// From what I've read, the overflow result of decimal math is undefined,
		// or useless, or varies between models of the 6502.  I'm just not going to
		// touch it for now.
		P.ScrozzleOverflow();
	}
	else
	{
		int unsignedResult = A + value + (P.C ? 1 : 0);
		int signedResult = (int8_t)A + (int8_t)value + (P.C ? 1 : 0);

		A = (uint8_t)unsignedResult;
		P.SetOverflow(signedResult > 127 || signedResult < -128);
		P.C = unsignedResult > 255;
	}

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
      PC = (uint16_t)(PC + branchDistance);
}

void CPU6502::BCS(void)
{
   int branchDistance = (int8_t)bus->ReadByte(PC++);
   if (P.C)
		PC = (uint16_t)(PC + branchDistance);
}

void CPU6502::BEQ(void)
{
   int branchDistance = (int8_t)bus->ReadByte(PC++);
   if (P.Z)
		PC = (uint16_t)(PC + branchDistance);
}

void CPU6502::BIT(uint16_t address)
{
   uint8_t memoryValue = bus->ReadByte(address);
   P.Z = (A & memoryValue) == 0;
   P.N = (memoryValue & 0x80) != 0;
   P.SetOverflow((memoryValue & 0x40) != 0);
}

void CPU6502::BMI(void)
{
   int branchDistance = (int8_t)bus->ReadByte(PC++);
   if (P.N)
		PC = (uint16_t)(PC + branchDistance);
}

void CPU6502::BNE(void)
{
   int branchDistance = (int8_t)bus->ReadByte(PC++);
   if (!P.Z)
		PC = (uint16_t)(PC + branchDistance);
}

void CPU6502::BPL(void)
{
   int branchDistance = (int8_t)bus->ReadByte(PC++);
   if (!P.N)
		PC = (uint16_t)(PC + branchDistance);
}

void CPU6502::BVC(void)
{
	int branchDistance = (int8_t)bus->ReadByte(PC++);
	if (!P.GetOverflow())
		PC = (uint16_t)(PC + branchDistance);
}

void CPU6502::BVS(void)
{
	int branchDistance = (int8_t)bus->ReadByte(PC++);
	if (P.GetOverflow())
		PC = (uint16_t)(PC + branchDistance);
}

void CPU6502::CMP(uint16_t address)
{
   Compare(A, bus->ReadByte(address));
}

void CPU6502::CPX(uint16_t address)
{
	Compare(X, bus->ReadByte(address));
}

void CPU6502::CPY(uint16_t address)
{
	Compare(Y, bus->ReadByte(address));
}

void CPU6502::DEC(uint16_t address)
{
   uint8_t result = (uint8_t)(bus->ReadByte(address) - 1);
   bus->WriteByte(address, result);
   SetNZ(result);
}

void CPU6502::DEY(void)
{
   SetNZ(--Y);
}

void CPU6502::EOR(uint16_t address)
{
   A ^= bus->ReadByte(address);
   SetNZ(A);
}

void CPU6502::INC(uint16_t address)
{
   uint8_t result = (uint8_t)(bus->ReadByte(address) + 1);
   bus->WriteByte(address, result);
   SetNZ(result);
}

void CPU6502::INY(void)
{
   SetNZ(++Y);
}


void CPU6502::JSR(uint16_t address)
{
   uint16_t returnAddress = (uint16_t)(PC - 1);
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

void CPU6502::LSR(uint16_t address)
{
	bus->WriteByte(address, DoLSR(bus->ReadByte(address)));
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

void CPU6502::RTI(void)
{
	P.FromByte(Pull());
	PC = (uint16_t)(Pull() + 256 * Pull());
}

void CPU6502::RTS(void)
{
	PC = (uint16_t)(Pull() + 256 * Pull() + 1);
}

void CPU6502::SBC(uint16_t address)
{
	uint8_t value = bus->ReadByte(address);

	if (P.D)
	{
		uint8_t v = FromBCD(value);
		uint8_t a = FromBCD(A);
		int result = a - v - (P.C ? 0 : 1);
		if (result < 0)
		{
			result += 100;
			P.C = false;
		}
		else
		{
			P.C = true;
		}

		A = ToBCD((uint8_t)result);

		// From what I've read, the overflow result of decimal math is undefined,
		// or useless, or varies between models of the 6502.  I'm just not going to
		// touch it for now.
		P.ScrozzleOverflow();
	}
	else
	{
		int unsignedResult = A - value - (P.C ? 0 : 1);
		int signedResult = (int8_t)A - (int8_t)value - (P.C ? 0 : 1);

		A = (uint8_t)unsignedResult;
		P.SetOverflow(signedResult > 127 || signedResult < -128);
		P.C = unsignedResult >= 0;
	}

	SetNZ(A);
}

void CPU6502::STA(uint16_t address)
{
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

uint8_t CPU6502::FromBCD(uint8_t value)
{
	// verify... for now I don't know what the correct action is
	// for this
	uint8_t lowDigit = (uint8_t)(value & 0xF);
	uint8_t highDigit = (uint8_t)(value >> 4);
	if (lowDigit > 9 || highDigit > 9)
		throw CPU6502Exception("Invalid BCD value");
	return (uint8_t)(highDigit * 10 + lowDigit);
}

uint8_t CPU6502::ToBCD(uint8_t value)
{
	if (value > 99)
		throw CPU6502Exception("Invalid value for BCD");
	return (uint8_t)((value % 10) + 16 * (value / 10));
}

