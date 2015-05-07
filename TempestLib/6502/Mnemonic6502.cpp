
#include "stdafx.h"
#include "Mnemonic6502.h"

std::string GetMnemonicString(Mnemonic6502 _mnemonic)
{
   switch (_mnemonic)
   {
      case OP_BIT: return "BIT";
      case OP_BMI: return "BMI";
      case OP_BNE: return "BNE";
      case OP_BPL: return "BPL";
		case OP_BVC: return "BVC";
		case OP_BVS: return "BVS";
		case OP_DEY: return "DEY";
      case OP_EOR: return "EOR";
      case OP_INC: return "INC";
      case OP_INY: return "INY";
      case OP_STA: return "STA";
      case OP_UNK: return "UNK";
      default:  return "UNK";
   }
}

