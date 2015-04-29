/* 
 * File:   ProcessorStatusWord.h
 * Author: Randy
 *
 * Created on April 25, 2015, 2:21 PM
 */

#ifndef PROCESSORSTATUSWORD_H
#define	PROCESSORSTATUSWORD_H

class ProcessorStatusWord
{
public:
   bool  C;
   bool  D;
   bool  I;
   bool  N;
   bool  V;
   bool  Z;
   
   uint8_t ToByte(void) const {
      uint8_t result = 0;
      if (C) result |= 0x01;
      if (D) result |= 0x08;
      if (I) result |= 0x04;
      if (N) result |= 0x80;
      if (V) result |= 0x40;
      if (Z) result |= 0x02;
      return result;
   }
   
   void FromByte(uint8_t value) {
      C = (value & 0x01) != 0;
      D = (value & 0x08) != 0;
      I = (value & 0x04) != 0;
      N = (value & 0x80) != 0;
      V = (value & 0x40) != 0;
      Z = (value & 0x02) != 0;
   }
};


#endif	/* PROCESSORSTATUSWORD_H */

