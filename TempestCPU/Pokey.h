/* 
 * File:   Pokey.h
 * Author: Randy
 *
 * Created on April 25, 2015, 11:14 PM
 */

#ifndef POKEY_H
#define	POKEY_H

#include "SoundChannelState.h"
class AbstractArcadeGameControlPanelReader;
class AbstractSoundGenerator;

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif


class Pokey
{
public:
	Pokey(int baseSoundChannel);
	virtual ~Pokey(void);
   void SetTempestIO(AbstractSoundGenerator *_soundOutput) { this->soundOutput = _soundOutput; }
   void SetControlPanel(AbstractArcadeGameControlPanelReader *_controlPanel) { this->controlPanel = _controlPanel; }

	uint8_t ReadByte(uint16_t address);
   void    WriteByte(uint16_t address, uint8_t value, uint64_t busTime);

	virtual uint8_t GetALLPOT(void) { return 0; }

protected:
   AbstractSoundGenerator * GetSoundOutput(void) { return soundOutput; }
   AbstractArcadeGameControlPanelReader *GetControlPanel(void) { return controlPanel; }

private:
   uint8_t ALLPOT = 0;
   uint8_t SKCTLS = 0;
   int baseSoundChannel = 0;
   SoundChannelState soundChannelState[4];
   AbstractSoundGenerator *soundOutput = nullptr;
   AbstractArcadeGameControlPanelReader *controlPanel = nullptr;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif	/* POKEY_H */

