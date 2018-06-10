
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
   controlPanel = _controlPanel;
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
   // do our periodic things
   //currentGameIndex = controlPanel->GetEncoder() % games.size();

   // give the current game a solid timeslice
   for (int i=0; i<100; ++i)
      games[currentGameIndex]->SingleStep();
}

void VectorMultiGame::Start(void)
{
   for (int i = 0; i < games.size(); ++i)
      games[i]->Start();
}

