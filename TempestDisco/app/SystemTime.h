
#ifndef SYSTEMTIME_H
#define SYSTEMTIME_H

#include <stdint.h>

void InitializeSystemTime(void);

uint32_t GetAPB1PeripheralClockSpeed(void);
uint32_t GetAPB1TimerClockSpeed(void);
uint32_t GetMillisecondCount(void);

#endif
