
#ifndef AUDIODRIVER_H
#define AUDIODRIVER_H

#define TEMPEST_DISCO_SOUND_BUFFER_SAMPLE_COUNT 1024

void AudioDriverInit(void);
bool AudioDriverPopEmptyBuffer(int16_t **buffer, int *frameCount);

#endif
