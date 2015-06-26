
#include "TempestDisco.h"
#include "RCCUtil.h"

uint32_t GetAPB1TimerClockSpeed(void)
{
	// get clock info
	RCC_ClocksTypeDef clockInfo;
	RCC_GetClocksFreq(&clockInfo);

	// the doc says that the clock for the timers is *2 if
	// the peripheral clock's pre-scale is not 1, so that tells
	// me this is correct
	if (clockInfo.PCLK1_Frequency != clockInfo.HCLK_Frequency)
		return clockInfo.PCLK1_Frequency * 2;
	else
		return clockInfo.PCLK1_Frequency;
}
