
#include "stdafx.h"
#include "SoundChannelState.h"
#include "AbstractSoundGenerator.h"

AbstractSoundGenerator::AbstractSoundGenerator(void)
{
}

AbstractSoundGenerator::~AbstractSoundGenerator(void)
{
}

void AbstractSoundGenerator::AllSoundOff(void)
{
   SoundChannelState state;
   for (int i = 0; i<8; ++i)
      SetSoundChannelState(i, state);
}

/// <summary>
/// Synchronizes our sound generation with the processor based on the given number
/// of clock cycles; the clock frequency is currently fixed at 1.5 MHz
/// <summary>
void AbstractSoundGenerator::SetCPUTime(uint64_t clockCycles)
{
   // see how long it has been since last time
   uint64_t elapsed = clockCycles - cpuTime;

   // if we are way off it means we somehow got out of sync with the CPU (and there are
   // valid reasons for this, such as switching betweel CPUs in a multi-game scenario);
   // in such a case we don't report the interval
   if (elapsed < 1000000) // 2/3 of a second at 1.5MHz
      ProcessTimeLapse(elapsed);

   // record the time
   cpuTime = clockCycles;
}
