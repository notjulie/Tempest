
#include "TempestDisco.h"
#include "WatchDog.h"

static int wwdgTimerCounts;

void InitializeWatchdog(void)
{
	// We use both watchdogs... the window watch dog is what
	// we use primarily, but the independent watchdog runs on
	// a separate clock so it's just there as a sanity check in case
	// things go really wrong.

	// Initialize the window watchdog... we actually clear this one
	// ourself in the window watchdog's own interrupt, so the only
	// way it would reset the processor is if our interrupt got
	// starved.  So the idea here is for the interrupt handler to
	// check for worrisome conditions and reset the timer.  Basically
	// it's a software watchdog, but one that has a hardware watchdog
	// as a failsafe.

	// get clock info
	RCC_ClocksTypeDef clockInfo;
	RCC_GetClocksFreq(&clockInfo);

	// figure out how to program our counter's duration
	uint32_t watchdogPeriodMicroseconds = 1000;
	uint32_t wwdgMasterFrequency = clockInfo.PCLK1_Frequency / 4096;
	uint32_t wwdgClockTicksPerPeriod = watchdogPeriodMicroseconds * wwdgMasterFrequency / 1000000;

	// we are allowed choices of 1, 2, 4, 8 as a prescaler and a range of 1 to 64
	// for the number of timer counts to somehow come up with a product close to
	// wwdgClockTicksPerPeriod
	int preScaleIndex;
	if (wwdgClockTicksPerPeriod == 0)
	{
		preScaleIndex = 0;
		wwdgTimerCounts = 1;
	}
	if (wwdgClockTicksPerPeriod <= 64)
	{
		preScaleIndex = 0;
		wwdgTimerCounts = wwdgClockTicksPerPeriod - 1;
	}
	else if (wwdgClockTicksPerPeriod <= 128)
	{
		preScaleIndex = 1;
		wwdgTimerCounts = wwdgClockTicksPerPeriod/2 - 1;
	}
	else if (wwdgClockTicksPerPeriod <= 256)
	{
		preScaleIndex = 2;
		wwdgTimerCounts = wwdgClockTicksPerPeriod/4 - 1;
	}
	else if (wwdgClockTicksPerPeriod <= 512)
	{
		preScaleIndex = 3;
		wwdgTimerCounts = wwdgClockTicksPerPeriod/8 - 1;
	}
	else
	{
		preScaleIndex = 3;
		wwdgTimerCounts = 64;
	}

	// enable its clock
   RCC->APB1ENR |= RCC_APB1ENR_WWDGEN;

   // set its config register:
   //    - early wakeup interrupt enabled
   //    - prescaler as calculated above
   //    - window value
   WWDG->CFR = (1<<9) | (preScaleIndex<<7) | 0x41;

   // set it
   WWDG->CR = (1<<7) | (wwdgTimerCounts - 1);
}

