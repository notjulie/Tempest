
#include "TempestDisco.h"

#include "SystemError.h"

#include "SystemTime.h"

static uint32_t millisecondCount;

void InitializeSystemTime(void)
{
	// ---------- SysTick timer -------- //
	// our systick clock is an even 1ms
	if (SysTick_Config(SystemCoreClock / 1000))
		ReportSystemError(SYSTEM_ERROR_SYSTICK_FAIL);
}

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

uint32_t GetAPB1PeripheralClockSpeed(void)
{
	// get clock info
	RCC_ClocksTypeDef clockInfo;
	RCC_GetClocksFreq(&clockInfo);
	return clockInfo.PCLK1_Frequency;
}



uint32_t GetMillisecondCount(void)
{
	return millisecondCount;
}

extern "C" {

	/**
	  * @brief  This function handles SysTick Handler.
	  * @param  None
	  * @retval None
	  */
	void SysTick_Handler(void)
	{
		++millisecondCount;
	}

};
