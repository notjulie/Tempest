//
//  AbstractTempestDB.h
//  iTempest
//
//  Created by Randy Rasmussen on 3/17/19.
//  Copyright © 2019 Randy Rasmussen. All rights reserved.
//

#ifndef AbstractTempestDB_h
#define AbstractTempestDB_h

class HighScoreList;

class AbstractTempestDB {
public:
   virtual ~AbstractTempestDB(void) {}
   
   virtual bool LoadHighScores(HighScoreList &);
   virtual void SaveHighScores(const HighScoreList &highScores);
};

#endif /* AbstractTempestDB_h */
