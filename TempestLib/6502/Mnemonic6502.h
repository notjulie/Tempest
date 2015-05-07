/* 
 * File:   Menmonic.h
 * Author: Randy
 *
 * Created on April 26, 2015, 8:07 AM
 */

#ifndef MENMONIC_H
#define	MENMONIC_H


enum Mnemonic6502 {
   OP_BIT,
   OP_BMI,
   OP_BNE,
   OP_BPL,
	OP_BVC,
	OP_BVS,
	OP_DEY,
   OP_EOR,
   OP_INC,
   OP_INY,
   OP_STA,
   OP_UNK
};

std::string GetMnemonicString(Mnemonic6502 _mnemonic);


#endif	/* MENMONIC_H */

