
#include "TempestDisco.h"
#include "LED.h"

#define LED_GREEN_PIN                    GPIO_Pin_12
#define LED_ORANGE_PIN                   GPIO_Pin_13
#define LED_RED_PIN                      GPIO_Pin_14
#define LED_BLUE_PIN                     GPIO_Pin_15

struct LEDDefinition
{
	uint16_t  gpioPin;
	volatile uint32_t  *ccr;
};

static bool usingPWM = false;
static const LEDDefinition LEDs[] = {
		{ LED_RED_PIN,    &TIM4->CCR3 },
		{ LED_GREEN_PIN,  &TIM4->CCR1 },
		{ LED_BLUE_PIN,   &TIM4->CCR4 },
		{ LED_ORANGE_PIN, &TIM4->CCR2 }
};

void InitializeLEDs(bool usePWM)
{
	// note how we are set up
	usingPWM = usePWM;

	// enable the peripheral clock for GPIOD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
   GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN|LED_ORANGE_PIN|LED_RED_PIN|LED_BLUE_PIN;
	GPIO_InitStructure.GPIO_Mode = usePWM ? GPIO_Mode_AF : GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// if we're not using PWM then we're done
	if (!usingPWM)
		return;

	// set the pins to connect to TIM4 outputs
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

   // the outputs are conveniently channels 1-4 of TIM4, so it's not
   // that much trouble to set them up as PWM
	// enable TIM4's peripheral clock
   RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

   // set TIM4 as an up counter with no surprises, currently disabled
   TIM4->CR1 = 0x80;

   // no surprises in CR2
   TIM4->CR2 = 0;

   // not in slave mode
   TIM4->SMCR = 0;

   // no DMA interrupts
   TIM4->DIER = 0;

   // don't generate any events
   TIM4->EGR = 0;

   // disable output compare
   TIM4->CCER = 0;

   // channel 1 and channel 2 configured as PWM outputs
   TIM4->CCMR1 = 0x6868;

   // channel 3 and channel 4 configured as PWM outputs
   TIM4->CCMR2 = 0x6868;

   // enable output compare for channels 1, 2, 3 & 4
   TIM4->CCER = 0x1111;

   // prescale
   TIM4->PSC = 0;

   // our auto reload value
   TIM4->ARR = 0xFFFF;

   // set all our channels to zero
   TIM4->CCR1 = 0;
   TIM4->CCR2 = 0;
   TIM4->CCR3 = 0;
   TIM4->CCR4 = 0;

   // enable output
   TIM4->BDTR = 0x8000;

   // no DMA
   TIM4->DCR = 0;

   // enable timer 4
   TIM4->CR1 |= 1;
}


void LEDOn(DiscoLED led, bool on)
{
	LEDValue(led, on ? 0xFFFF : 0);
}

void LEDValue(DiscoLED led, uint16_t value)
{
	if (usingPWM)
		*LEDs[led].ccr = value;
	else
		GPIO_WriteBit(GPIOD, LEDs[led].gpioPin, (value!=0) ? Bit_SET : Bit_RESET);
}

