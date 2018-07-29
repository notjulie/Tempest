// ====================================================================
// Simple sound generator
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This plays part of a simple sound generator, whose genesis was
//    the emulation of the sound generation of the Atari Pokey chip
//    used in Tempest.  In my hardware setup I have a serial connection
//    between the main processor and the sound generator, so this
//    object essentially serves as a command sent on the serial line.
//    Thus this class has comparison operators so that I don't waste
//    serial clock cycles updating channels whose state hasn't changed.
//
// NOTE:
//    This class is used in an embedded controller, so no C++11
//    syntax, please.
// ====================================================================

#ifndef SOUNDCHANNELSTATE_H
#define SOUNDCHANNELSTATE_H

#include <stdint.h>

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

enum Waveform {
   // NOTE!!! Currently the waveform value must be in the range 0..15, as the serial
   // protocol's current implementation shoves them into a single byte
   None,
   Noise4Bit,
   Noise5BitHalfFrequency,
   Noise17Bit,
   SquareWave
};

class SoundChannelState
{
public:
   SoundChannelState(void);

   float GetChannelFrequency(void) const { return channelFrequency; }
   uint8_t GetVolume(void) const { return volume; }
   Waveform GetWaveform(void) const { return waveform; }
   void SetChannelFrequency(float _frequency) { this->channelFrequency = _frequency; }
   void SetVolume(uint8_t _volume) { this->volume = _volume; }
   void SetWaveform(Waveform _waveform) { this->waveform = _waveform; }

   bool operator==(const SoundChannelState &state) const;
   bool operator!=(const SoundChannelState &state) const;

private:
   float channelFrequency;
   uint8_t volume;
   Waveform waveform;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
