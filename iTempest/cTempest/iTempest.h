//
//  iTempest.h
//  iTempest
//
//  Created by Randy Rasmussen on 11/12/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#ifndef iTempest_h
#define iTempest_h

// generic Tempest includes
#include "AbstractTempestEnvironment.h"
#include "SimpleVector.h"
#include "TempestRunner.h"

// iTempest includes
#include "iTempestSoundOutput.h"
#include "TempestVector.h"


class iOSTempestEnvironment : public AbstractTempestEnvironment
{
public:
    virtual std::string GetDatabasePathName(void) { return ":memory:"; }
};

class iTempest
{
public:
   iTempest(void);

   int GetVectors(TempestVector *buffer, int bufferSize);
   bool IsInAttractMode(void) { return runner.IsInAttractMode(); }

   // dispatchers to the control panel
   bool GetPlayer1LEDState(void) { return controlPanel.GetPlayer1LEDState(); }
   bool GetPlayer2LEDState(void) { return controlPanel.GetPlayer2LEDState(); }
   void MoveSpinner(int offset) { controlPanel.MoveSpinner(offset); }
   void SetPlayer1ButtonState(bool state) { controlPanel.SetInputButtonState(ONE_PLAYER_BUTTON, state); }
   void SetPlayer2ButtonState(bool state) { controlPanel.SetInputButtonState(TWO_PLAYER_BUTTON, state); }
   void SetFireButtonState(bool state) { controlPanel.SetInputButtonState(FIRE_BUTTON, state); }
   void SetZapButtonState(bool state) { controlPanel.SetInputButtonState(ZAPPER_BUTTON, state); }

private:
   iOSTempestEnvironment environment;
   iTempestSoundOutput soundOutput;
   TempestRunner runner;
   SimpleArcadeGameControlPanel controlPanel;
};

#endif /* iTempest_hpp */
