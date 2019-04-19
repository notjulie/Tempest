
#include "TempestDisco.h"

#include "SystemError.h"
#include "SystemTime.h"

#include "WatchDog.h"



static int wwdgTimerCounts;

static void InitializeIndependentWatchdog(void);
static void InitializeWindowWatchdog(void);

static bool heartBeat = false;

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
	uint32_t wwdgMasterFrequency = GetAPB1TimerClockSpeed() / 4096;
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
		wwdgTimerCounts = wwdgClockTicksPerPeriod;
	}
	else if (wwdgClockTicksPerPeriod <= 128)
	{
		preScaleIndex = WWDG_Prescaler_2;
		wwdgTimerCounts = wwdgClockTicksPerPeriod/2;
	}
	else if (wwdgClockTicksPerPeriod <= 256)
	{
		preScaleIndex = WWDG_Prescaler_4;
		wwdgTimerCounts = wwdgClockTicksPerPeriod/4;
	}
	else if (wwdgClockTicksPerPeriod <= 512)
	{
		preScaleIndex = WWDG_Prescaler_8;
		wwdgTimerCounts = wwdgClockTicksPerPeriod/8;
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

   // this would be one more than this, but our interrupt comes one tick
   // before timeout... basically we are trying to roughly set the
   // frequency of the interrupt
   WWDG_Enable(64 + wwdgTimerCounts);

   WWDG_ClearFlag();
   WWDG_EnableIT();
}

bool WatchdogHeartbeat(void)
{
	return heartBeat;
}


extern "C" {
	void WWDG_IRQHandler(void)
	{
		// reset the counter
	   WWDG->CR = (1<<7) | (64 + wwdgTimerCounts);

	   // set the orange LED as about a one second heart beat... this assumes a
	   // 1ms period
	   static unsigned int count = 0;
	   heartBeat = ((++count % 1000) > 500) != 0;

	   // clear the interrupt
		WWDG->SR = 0;
	}
};
