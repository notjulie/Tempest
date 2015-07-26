
#include "TempestDisco.h"

#include "DiscoRGB.h"

#include "DiscoVector.h"


DiscoVector Vector;



DiscoVector::DiscoVector(void)
{
	// create our default vectors to be a simple diamond that maxes at
	// half our dynamic range
	defaultVectorList[0].startX = -16000;
	defaultVectorList[0].endX = 0;
	defaultVectorList[0].startY = 0;
	defaultVectorList[0].endY = 16000;
	defaultVectorList[0].color = 0;

	defaultVectorList[1].startX = 0;
	defaultVectorList[1].endX = 16000;
	defaultVectorList[1].startY = 16000;
	defaultVectorList[1].endY = 0;
	defaultVectorList[1].color = 0;

	defaultVectorList[2].startX = 16000;
	defaultVectorList[2].endX = 0;
	defaultVectorList[2].startY = 0;
	defaultVectorList[2].endY = -16000;
	defaultVectorList[2].color = 0;

	defaultVectorList[3].startX = 0;
	defaultVectorList[3].endX = -16000;
	defaultVectorList[3].startY = -16000;
	defaultVectorList[3].endY = 0;
	defaultVectorList[3].color = 0;

	// initialize our vector list
	currentVectors = defaultVectorList;
	currentVectorCount = 4;
}

void DiscoVector::Init(void)
{
	// initialize the DACs for XY
	dac1.Init(1);
	dac2.Init(2);

	// initialize RGB
	InitializeRGB();
}


int DiscoVector::CreateRamp(uint16_t *rampBuffer, uint16_t from, uint16_t to)
{
   // create the ramp
	int index = 0;
	int value = from;
	for (;;)
	{
		rampBuffer[index++] = value;
		if (from < to)
		{
			if (++value > to)
				break;
		}
		else
		{
			if (--value < to)
				break;
		}
	}
	int sampleCount = index;
	return sampleCount;
}

void DiscoVector::Service(void)
{
	bool dac1Running = dac1.IsDMARunning();
	bool dac2Running = dac2.IsDMARunning();
	if (dac1Running || dac2Running)
		return;

	// reset DMA1... ugly hack for now... will figure out the
	// gentler way to do this later
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC, DISABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC, DISABLE);

	int usDuration = 5000000;

	static unsigned phase = 0;
	switch ((++phase) & 3)
	{
	case 0:
		CreateRamp(xDacBuffer, 1024, 2048);
		CreateRamp(yDacBuffer, 2048, 3072);
		break;
	case 1:
		CreateRamp(xDacBuffer, 2048, 3072);
		CreateRamp(yDacBuffer, 3072, 2048);
		break;
	case 2:
		CreateRamp(xDacBuffer, 3072, 2048);
		CreateRamp(yDacBuffer, 2048, 1024);
		break;
	case 3:
		CreateRamp(xDacBuffer, 2048, 1024);
		CreateRamp(yDacBuffer, 1024, 2048);
		break;
	}

	dac1.StartRamp(xDacBuffer, 1024, usDuration);
	dac2.StartRamp(yDacBuffer, 1024, usDuration);
}
