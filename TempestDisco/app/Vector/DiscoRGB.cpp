
#include "TempestDisco.h"
#include "DiscoRGB.h"

// There is a great deal of caution written and designed into this so that
// the RGB outputs don't get left on in such a way as to burn the CRT.  From
// the hardware side:
//    - the RGB outputs have pulldown resistors to keep them low on powerup
//    - the RGB amplifier's power supply is disabled on powerup; we enable it
//      here in software
//    - the Tempest hardware has a spot killer circuit
// Other software modules help by doing this:
//    - X and Y have a screen saver type pattern that they follow when they
//      are otherwise idle, so that even if the RGB outputs turn on unexpectedly
//      they will not burn a single point
// Then for our part, we are implemented as a one-shot, so that instead of just
// being told to turn on, we are told to turn on for a given time period, so that
// if the vector driver doesn't shut RGB off before then our timer does.  And our
// timer is the WWDG watchdog interrupt, so if our timer fails the processor will
// reset, killing our output, our power supply, etc.

// The remaining concern would be if the XY driver kept drawing to the same
// area all the time.  That's a more complicated issue, but one that I have
// plans for.


#define RGB_MAX_VALUE 16

void InitializeRGB(void)
{
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

   // make sure our PWMs start out at zero
   TIM1->CCR1 = 0;
   TIM1->CCR2 = 0;
   TIM1->CCR3 = 0;

   // connect TIM1 to the outputs
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);

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
