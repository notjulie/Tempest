
#include "TempestDisco.h"
#include "WatchDog.h"

static int wwdgTimerCounts;

static void InitializeWindowWatchdog(void)
{
	// Initialize the window watchdog... we actually clear this one
	// ourself in the window watchdog's own interrupt, so the only
	// way it would reset the processor is if our interrupt got
	// starved.  So the idea here is for the interrupt handler to
	// check for worrisome conditions and reset the timer.  Basically
	// it's a software watchdog, but one that has a hardware watchdog
	// as a failsafe.

   // enable the WWDG interrupt
	NVIC_InitTypeDef   NVIC_InitStructure;
   NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

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
		preScaleIndex = WWDG_Prescaler_1;
		wwdgTimerCounts = 1;
	}
	if (wwdgClockTicksPerPeriod <= 64)
	{
		preScaleIndex = WWDG_Prescaler_1;
		wwdgTimerCounts = wwdgClockTicksPerPeriod - 1;
	}
	else if (wwdgClockTicksPerPeriod <= 128)
	{
		preScaleIndex = WWDG_Prescaler_2;
		wwdgTimerCounts = wwdgClockTicksPerPeriod/2 - 1;
	}
	else if (wwdgClockTicksPerPeriod <= 256)
	{
		preScaleIndex = WWDG_Prescaler_4;
		wwdgTimerCounts = wwdgClockTicksPerPeriod/4 - 1;
	}
	else if (wwdgClockTicksPerPeriod <= 512)
	{
		preScaleIndex = WWDG_Prescaler_8;
		wwdgTimerCounts = wwdgClockTicksPerPeriod/8 - 1;
	}
	else
	{
		preScaleIndex = WWDG_Prescaler_8;
		wwdgTimerCounts = 64;
	}

	// enable its clock
   RCC->APB1ENR |= RCC_APB1ENR_WWDGEN;

   WWDG_SetPrescaler(preScaleIndex);
   WWDG_SetWindowValue(127);
   WWDG_Enable(64 + wwdgTimerCounts - 1);
   WWDG_ClearFlag();
   WWDG_EnableIT();
}

static void InitializeIndependentWatchdog(void)
{
	// this is the secret handshake to allow access to its registers
	IWDG->KR = 0x5555;

	// set the prescaler to 4
	IWDG->PR = 0;

	// set the reload register so that the clock signals about every
	// 100ms... this is perfectly fine, as I say the window watchdog is our
	// workhorse... this is just a sanity check
	IWDG->RLR = 800;

	// this starts the watchdog
	IWDG->KR = 0xCCCC;
}

void InitializeWatchdog(void)
{
	// We use both watchdogs... the window watch dog is what
	// we use primarily, but the independent watchdog runs on
	// a separate clock so it's just there as a sanity check in case
	// things go really wrong.
	InitializeWindowWatchdog();
	InitializeIndependentWatchdog();
}

void ResetIndependentWatchdogCounter(void)
{
	IWDG->KR = 0xAAAA;
}

extern "C" {
	void WWDG_IRQHandler(void)
	{
		// reset the counter
	   WWDG->CR = (1<<7) | (64 + wwdgTimerCounts - 1);

	   // clear the interrupt
		WWDG->SR = 0;
	}
};
