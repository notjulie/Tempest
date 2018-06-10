
#include "AbstractTempestIO.h"
#include "VectorMultiGame.h"

VectorMultiGame::VectorMultiGame(std::initializer_list<std::function<VectorGame *()>> _games)
{
   try
   {
      // use the passed-in functions to instantiate our games
      for (auto g : _games)
         games.push_back(g());
   }
   catch (...)
   {
      DeleteGames();
      throw;
   }
}

VectorMultiGame::~VectorMultiGame(void)
{
   DeleteGames();
}


void VectorMultiGame::DeleteGames(void)
{
   for (int i = 0; i < games.size(); ++i)
      delete games[i];

   games.resize(0);
}

CPU6502Runner *VectorMultiGame::GetCPURunner(void)
{
   return games[currentGameIndex]->GetCPURunner();
}

void VectorMultiGame::GetAllVectors(std::vector<SimpleVector> &vectors)
{
   games[currentGameIndex]->GetAllVectors(vectors);
}

void VectorMultiGame::SetControlPanel(AbstractArcadeGameControlPanelReader *_controlPanel)
{
   // save a reference to it for our own purposes
   controlPanel = _controlPanel;

   // pass it along to the games
   for (int i = 0; i < games.size(); ++i)
      games[i]->SetControlPanel(controlPanel);
}

void VectorMultiGame::SetSoundOutput(AbstractTempestSoundOutput *soundOutput)
{
   for (int i = 0; i < games.size(); ++i)
      games[i]->SetSoundOutput(soundOutput);
}

void VectorMultiGame::SingleStep(void)
{
   // on a rising edge of the menu button we do our menu thing
   bool newMenuButtonState = (controlPanel->GetButtons() & MENU_BUTTON) != 0;
   if (newMenuButtonState && !menuButtonState)
   {
      if (++currentGameIndex >= games.size())
         currentGameIndex = 0;
   }
   menuButtonState = newMenuButtonState;

   // give the current game a solid timeslice by stepping it a bunch of times;
   // this is just because the games need to be stepped at a very high frequency
   // (think processor frequency), far more often than we need to do our little
   // overhead items in this function
   for (int i=0; i<100; ++i)
      games[currentGameIndex]->SingleStep();
}

void VectorMultiGame::Start(void)
{
   for (int i = 0; i < games.size(); ++i)
      games[i]->Start();
}

