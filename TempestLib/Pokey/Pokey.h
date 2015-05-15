/* 
 * File:   Pokey.h
 * Author: Randy
 *
 * Created on April 25, 2015, 11:14 PM
 */

#ifndef POKEY_H
#define	POKEY_H

#include "SoundChannel.h"

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures


class Pokey
{
public:
	Pokey(void);
	virtual ~Pokey(void);

	uint8_t ReadByte(uint16_t address);
	void    WriteByte(uint16_t address, uint8_t value);

	void AddWaveData(int16_t *buffer, int count);

	virtual uint8_t GetALLPOT(void) { return 0; }

public:
	static const bool *Get5BitNoise(void) { return poly5; }
	static const bool *Get17BitNoise(void) { return poly17; }

private:
	static void InitializeNoiseBuffers(void);
	static void MakeNoise(bool *buffer, int count);

private:
	uint8_t ALLPOT;
	SoundChannel	sound1;
	SoundChannel	sound2;
	SoundChannel	sound3;
	SoundChannel	sound4;

private:
	static bool noiseWaveformsInitialized;
	static bool poly5[32];
	static bool poly17[128 * 1024];
};


#pragma warning(pop)

#endif	/* POKEY_H */

