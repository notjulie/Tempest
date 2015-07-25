
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


void DiscoVector::Service(void)
{
	bool dac1Running = dac1.IsDMARunning();
	bool dac2Running = dac2.IsDMARunning();
	if (!dac1Running && !dac2Running)
	{
		dac1.StartRamp(0, 4095, 10000000);
		dac2.StartRamp(0, 4095, 10000000);
	}
}
