/* 
 * File:   VectorGenerator.h
 * Author: Randy
 *
 * Created on April 26, 2015, 12:42 PM
 */

#ifndef VECTORGENERATOR_H
#define	VECTORGENERATOR_H

#include <stdint.h>
#include <vector>

class VectorGenerator
{
public:
   VectorGenerator(void);
   
   void    Go(void);
   bool    IsVectorRAMAddress(uint16_t address) const;
   bool    IsVectorROMAddress(uint16_t address) const;
   void    LoadROM(uint16_t address, uint8_t *buffer, int count);
   uint8_t ReadVectorRAM(uint16_t address);
   uint8_t ReadVectorROM(uint16_t address);
   void    Reset(void);
   void    WriteVectorRAM(uint16_t address, uint8_t value);
   
private:
   uint8_t GetAt(uint16_t pcOffset);
   bool SingleStep(void);
   
private:   
   std::vector<uint8_t>  vectorRAM;
   std::vector<uint8_t>  vectorROM;
   uint16_t PC;
   std::vector<uint16_t> stack;
};

#endif	/* VECTORGENERATOR_H */

