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
	ProcessorStatusWord(void);

public:
   bool  C;
   bool  D;
   bool  I;
   bool  N;
   bool  Z;
   
public:
	bool GetOverflow(void);
	void ScrozzleOverflow(void);
	void SetOverflow(bool value);

	uint8_t ToByte(void) const;
	void FromByte(uint8_t value);

private:
	bool  v;
	bool  overflowSetByDecimalMath;
};


#endif	/* PROCESSORSTATUSWORD_H */

