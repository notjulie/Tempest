
#include "stdafx.h"
#include <stdio.h>
#include <pthread.h>

#include "TempestCPU/TempestRunner.h"

#include "TempestPiEnvironment.h"
#include "TempestPiIO.h"



int main()
{
    // create our peripherals
    TempestPiEnvironment environment;
    TempestPiIO io;

    // create the runner object that drives the fake 6502
    TempestRunner tempestRunner(&environment);
    tempestRunner.SetTempestIO(&io);

    // go
    tempestRunner.Start();

    // push to the screen
    for (;;)
        io.PushFrameToScreen();

    sleep(10);
    uint64_t clockCycles = tempestRunner.GetTotalClockCycles();
    printf("Total clock cycles: %ld\n", (int)clockCycles);
}
