
#include <stm32f4xx.h>
#include "TempestDisco.h"

#include "SystemTime.h"

#include "Serial.h"

void InitializeSerial(void)
{
	int baudRate = 115200;

	// enable clock for USART2 and GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// set PA2 and PA3 to alternate function 7 (USART2 TX & RX)
	GPIOA->AFR[0] &= ~0xFF00;
	GPIOA->AFR[0] |= 0x7700;
	GPIOA->MODER &= ~0xF0;
	GPIOA->MODER |= 0xA0;

	// get the UART master clock speed
	uint32_t masterClock = GetAPB1PeripheralClockSpeed();

	// divide by the baud rate... the doc tries to make it seem more
	// complicated than it really is
	double fClockDivider = masterClock;
	fClockDivider /= baudRate;
	int clockDivider = (int)(fClockDivider + 0.5);
	USART2->BRR = (uint16_t)clockDivider;

	// enable
	USART2->CR1 = 0x200C;
}


void ServiceSerial(void)
{
	USART2->DR = 'x';
}

