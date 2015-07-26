
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
	// initialize the DAC for XY
	dac.Init();

	// initialize RGB
	InitializeRGB();
}


int DiscoVector::CreateDualRamp(uint16_t fromX, uint16_t toX, uint16_t fromY, uint16_t toY)
{
	// for now it's just always 1024 uniform for a test
	int index = 0;
	int xValue = fromX;
	int yValue = fromY;
	for (;;)
	{
		// quit at 1024 for now
		if (index >= 1024)
			return index;

		// add the current values
		dacBuffer[index++] = (yValue<<16) + xValue;

		// increment/decrement/whatever
		if (xValue > toX)
			--xValue;
		else
			++xValue;
		if (yValue > toY)
			--yValue;
		else
			++yValue;
	}
}

void DiscoVector::Service(void)
{
	if (dac.IsDMARunning())
		return;

	// reset DMA1... ugly hack for now... will figure out the
	// gentler way to do this later
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC, DISABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC, DISABLE);

	int usDuration = 5000000;

	static unsigned phase = 0;

	int sampleCount;
	switch ((++phase) & 3)
	{
	case 0:
		sampleCount = CreateDualRamp(1024, 2048, 2048, 3072);
		break;
	case 1:
		sampleCount = CreateDualRamp(2048, 3072, 3072, 2048);
		break;
	case 2:
		sampleCount = CreateDualRamp(3072, 2048, 2048, 1024);
		break;
	case 3:
		sampleCount = CreateDualRamp(2048, 1024, 1024, 2048);
		break;
	}

	dac.StartOutput(dacBuffer, sampleCount, usDuration);
}
