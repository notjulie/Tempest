
#ifndef WAVESOUNDDRIVER_H
#define WAVESOUNDDRIVER_H

class WaveSoundSource;

/// <summary>
/// Abstract class that a caller implements to receive wave data
/// </summary>
class WaveSoundDriver
{
public:
   /// <summary>
   /// Called frequently to see if the sound device has any output buffers
   /// to be filled.  For Tempest, this "pushing" model makes sense, since the
   /// wave output is constantly being updated.
   /// </summary>
   virtual void FillNextBuffer(WaveSoundSource *streamer) = 0;
};

#endif
