
#include "TempestDisco.h"
#include "LED.h"

#define LED_GREEN_PIN                    GPIO_Pin_12
#define LED_GREEN_GPIO_PORT              GPIOD
#define LED_GREEN_GPIO_CLK               RCC_AHB1Periph_GPIOD

#define LED_ORANGE_PIN                   GPIO_Pin_13
#define LED_ORANGE_GPIO_PORT             GPIOD
#define LED_ORANGE_GPIO_CLK              RCC_AHB1Periph_GPIOD

#define LED_RED_PIN                      GPIO_Pin_14
#define LED_RED_GPIO_PORT                GPIOD
#define LED_RED_GPIO_CLK                 RCC_AHB1Periph_GPIOD

#define LED_BLUE_PIN                     GPIO_Pin_15
#define LED_BLUE_GPIO_PORT               GPIOD
#define LED_BLUE_GPIO_CLK                RCC_AHB1Periph_GPIOD


void InitializeLEDs(void)
{
	// enable the peripheral clock for GPIOD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
   GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN|LED_ORANGE_PIN|LED_RED_PIN|LED_BLUE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void LEDRed(bool on)
{
	GPIO_WriteBit(LED_RED_GPIO_PORT, LED_RED_PIN, on ? Bit_SET : Bit_RESET);
}

void LEDGreen(bool on)
{
	GPIO_WriteBit(LED_GREEN_GPIO_PORT, LED_GREEN_PIN, on ? Bit_SET : Bit_RESET);
}

void LEDBlue(bool on)
{
	GPIO_WriteBit(LED_BLUE_GPIO_PORT, LED_BLUE_PIN, on ? Bit_SET : Bit_RESET);
}

void LEDOrange(bool on)
{
	GPIO_WriteBit(LED_ORANGE_GPIO_PORT, LED_ORANGE_PIN, on ? Bit_SET : Bit_RESET);
}
