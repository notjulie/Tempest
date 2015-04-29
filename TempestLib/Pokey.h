/* 
 * File:   Pokey.h
 * Author: Randy
 *
 * Created on April 25, 2015, 11:14 PM
 */

#ifndef POKEY_H
#define	POKEY_H

class Pokey
{
public:
   uint8_t ReadByte(uint16_t address) { return 0; }
   void    WriteByte(uint16_t address, uint8_t value) {}
};

#endif	/* POKEY_H */

