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


#include "stdafx.h"
#include "SoundChannelState.h"

SoundChannelState::SoundChannelState(void)
{
   frequency = 0;
   volume = 0;
   waveform = None;
}


bool SoundChannelState::operator==(const SoundChannelState &state) const
{
   // if they both have zero volume the are equal
   if (state.volume == 0 && this->volume == 0)
      return true;

   // else everything has to match
   return
      state.frequency == this->frequency &&
      state.volume == this->volume &&
      state.waveform == this->waveform;
}

bool SoundChannelState::operator!=(const SoundChannelState &state) const
{
   return !operator==(state);
}

