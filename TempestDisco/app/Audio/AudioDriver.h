// ====================================================================
// Tempest emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is a wrapper around a sample implementation of double-buffered
//    wave audio for the STM32F4 Discovery board, tailored to match my
//    design sensibilities.
// ====================================================================


#ifndef AUDIODRIVER_H
#define AUDIODRIVER_H

#define TEMPEST_DISCO_SOUND_BUFFER_SAMPLE_COUNT 1024

void AudioDriverInit(void);
bool AudioDriverPopEmptyBuffer(int16_t **buffer, int *frameCount);

#endif
