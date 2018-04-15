
#include <stm32f4xx.h>
#include "TempestDisco.h"

#include "SystemError.h"
#include "SystemTime.h"
#include "TempestIO/TempestMemoryStream.h"

#include "Serial.h"


// our global memory stream between the serial port and the app
static TempestMemoryStream memoryStream;
AbstractTempestStream &SerialStream = *memoryStream.GetLeftSide();

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
	USART2->CR1 = 0x202C;

	// enable USART2 interrupt handler
	NVIC_InitTypeDef   NVIC_InitStructure;
   NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}


void ServiceSerial(void)
{
	// send the next byte if we can... future version can be interrupt driven
	// if necessary, but we don't send that much data and aren't that urgent
	if (USART2->SR & 0x80)
	{
		int b = memoryStream.GetRightSide()->Read();
		if (b >= 0)
			USART2->DR = (uint8_t)b;
	}
}

extern "C" {
void USART2_IRQHandler(void)
{
	// handle receive interrupt
	if (USART2->SR & 0x20)
		memoryStream.GetRightSide()->Write(USART2->DR);
}
};

