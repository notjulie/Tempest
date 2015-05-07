/* 
 * File:   AbstractBus.h
 * Author: Randy
 *
 * Created on April 25, 2015, 2:11 PM
 */

#ifndef ABSTRACTBUS_H
#define	ABSTRACTBUS_H


class AbstractBus
{
public:
	virtual bool IsIRQ(void) = 0;
   virtual uint8_t ReadByte(uint16_t address) = 0;
   virtual void    WriteByte(uint16_t address, uint8_t value) = 0;
};

#endif	/* ABSTRACTBUS_H */

