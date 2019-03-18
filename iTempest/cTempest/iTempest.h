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
#include "AbstractGameEnvironment.h"
#include "SimpleVector.h"
#include "VectorGameRunner.h"
#include "TempestGame.h"

// iTempest includes
#include "iTempestSoundOutput.h"
#include "TempestVector.h"


class iTempest
{
public:
   iTempest(void);
   ~iTempest(void);

   int GetVectors(TempestVector *buffer, int bufferSize);
   bool IsInAttractMode(void) { return game->IsInAttractMode(); }

   // dispatchers to the control panel
   bool GetPlayer1LEDState(void) { return controlPanel.GetButtonLED(ONE_PLAYER_BUTTON); }
   bool GetPlayer2LEDState(void) { return controlPanel.GetButtonLED(TWO_PLAYER_BUTTON); }
   void MoveSpinner(int offset) { controlPanel.MoveEncoder(offset); }
   void SetPlayer1ButtonState(bool state) { controlPanel.SetButtonState(ONE_PLAYER_BUTTON, state); }
   void SetPlayer2ButtonState(bool state) { controlPanel.SetButtonState(TWO_PLAYER_BUTTON, state); }
   void SetFireButtonState(bool state) { controlPanel.SetButtonState(FIRE_BUTTON, state); }
   void SetZapButtonState(bool state) { controlPanel.SetButtonState(ZAPPER_BUTTON, state); }

private:
   AbstractGameEnvironment environment;
   iTempestSoundOutput soundOutput;
   VectorGameRunner *runner = nullptr;
   TempestGame *game = nullptr;
   SimpleArcadeGameControlPanel controlPanel;
};

#endif /* iTempest_hpp */
