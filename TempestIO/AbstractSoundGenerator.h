
#ifndef ABSTRACTSOUNDGENERATOR_H
#define ABSTRACTSOUNDGENERATOR_H

class SoundChannelState;

/// <summary>
/// Abstract sound generator class; it has its origins as an emulator of the
/// sound generation of Atari's POKEY chip, but has grown beyond that
/// </summary>
class AbstractSoundGenerator
{
public:
   AbstractSoundGenerator(void);
   virtual ~AbstractSoundGenerator(void);

   /// <summary>
   /// Extinguishes all channels
   /// </summary>
   void AllSoundOff(void);

   /// <summary>
   /// Set the state of a given sound channel
   /// </summary>
   virtual void SetSoundChannelState(int channel, SoundChannelState state) = 0;

   /// <summary>
   /// Synchronizes our sound generation with the processor based on the given number
   /// of clock cycles; the clock frequency is currently fixed at 1.5 MHz
   /// <summary>
   void SetCPUTime(uint64_t clockCycles);

protected:
   /// <summary>
   /// Inserts a delay of the given number of clock cycles, by which I mean a period
   /// during which all channels continue to generate sound based on their current parameters
   /// </summary>
   virtual void ProcessTimeLapse(uint64_t clockCycles) = 0;

private:
   uint64_t cpuTime;
};


#endif
