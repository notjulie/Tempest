
#include "TempestDisco.h"
#include "DiscoVector.h"

#define RGB_MAX_VALUE 16

DiscoVector Vector;

DiscoVector::DiscoVector(void)
{
}

void DiscoVector::Init(void)
{
	// initialize the DACs for XY
	dac1.Init(1);
	dac2.Init(2);
	dac1.StartRamp(0, 4095, 10000000);
	dac2.StartRamp(0, 4095, 10000000);

	// initialize our RGB PWMs... we use channel 1, 2 and 3 of TIM1
	// enable TIM1's peripheral clock
   RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

   // set it as an up counter with no surprises, currently disabled
   TIM1->CR1 = 0;

   // no surprises in CR2
   TIM1->CR2 = 0;

   // not in slave mode
   TIM1->SMCR = 0;

   // no DMA interrupts
   TIM1->DIER = 0;

   // don't generate any events
   TIM1->EGR = 0;

   // disable output compare
   TIM1->CCER = 0;

   // channel 1 and channel 2 configured as PWM outputs
   TIM1->CCMR1 = 0x6060;

   // channel 3 and channel 4 configured as PWM outputs
   TIM1->CCMR2 = 0x6060;

   // enable output compare for channels 1, 2 & 3
   TIM1->CCER = 0x0111;

   // no prescale... run it fast
   TIM1->PSC = 0;

   // our auto reload value
   TIM1->ARR = RGB_MAX_VALUE - 1;

   // set all our channels to zero
   TIM1->CCR1 = 0;
   TIM1->CCR2 = 0;
   TIM1->CCR3 = 0;

   // enable output
   TIM1->BDTR = 0x8000;

   // no DMA
   TIM1->DCR = 0;

   // enable timer 1
   TIM1->CR1 |= 1;

   // enable GPIOE
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

   // enable PE9, PE11 and PE13 as push/pull
   GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	// connect TIM1 to the outputs
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);

   // set the channels to something interesting for testing
   TIM1->CCR1 = RGB_MAX_VALUE / 4;
   TIM1->CCR2 = RGB_MAX_VALUE * 2 / 4;
   TIM1->CCR3 = RGB_MAX_VALUE * 4 / 4;

   // enable our +/- 15V supply by setting PC1 to zero
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIOC->BSRRH = (1<<1);
}

