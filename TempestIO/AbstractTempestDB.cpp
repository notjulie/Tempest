//
//  AbstractTempestDB.cpp
//  iTempest
//
//  Created by Randy Rasmussen on 3/17/19.
//  Copyright © 2019 Randy Rasmussen. All rights reserved.
//

#include "TempestIO.Headers.h"
#include "AbstractTempestDB.h"


bool AbstractTempestDB::LoadHighScores(HighScoreList &)
{
   return false;
}

void AbstractTempestDB::SaveHighScores(const HighScoreList &highScores)
{
}

