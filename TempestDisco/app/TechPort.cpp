
#include <stm32f4xx.h>
#include "SystemTime.h"
#include "TechPort.h"

TechPort Tech;

TechPort::TechPort(void)
{

}

void TechPort::Initialize(void)
{
	// USART6 TX is PC6 which is conveniently located at the end of one
	// of the headers

	int baudRate = 115200;

	// enable clock for USART6 and GPIOC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	// set PC6 to alternate function 8 (USART6 TX)
	GPIOA->AFR[0] &= ~(15<<24);
	GPIOA->AFR[0] |= (8<<24);
	GPIOA->MODER &= ~(3<<12);
	GPIOA->MODER |= (2<<12);

	// get the UART master clock speed
	uint32_t masterClock = GetAPB2PeripheralClockSpeed();

	// divide by the baud rate... the doc tries to make it seem more
	// complicated than it really is
	double fClockDivider = masterClock;
	fClockDivider /= baudRate;
	int clockDivider = (int)(fClockDivider + 0.5);
	USART6->BRR = (uint16_t)clockDivider;

	// enable
	USART6->CR1 = 0x202C;

	// enable USART6 interrupt handler
	NVIC_InitTypeDef   NVIC_InitStructure;
   NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

   // issue a hello
   WriteString("Tech port ready\r\n");
}

void TechPort::WriteString(const char *s)
{
	// add it to the buffer
	while (*s != 0)
		stream.Write(*s++);

	// make sure that our transmit interrupt is enabled
	USART6->CR1 |= (1<<7);
}

void TechPort::TransmitNext(void)
{
	int c = stream.Read();
	if (c >= 0)
		USART6->DR = c;
	else
		USART6->CR1 &= ~(1<<7);
}


extern "C" {
void USART6_IRQHandler(void)
{
	// handle transmit empty interrupt
	if (USART6->SR & 0x80)
	{
		Tech.TransmitNext();
	}
}
};

