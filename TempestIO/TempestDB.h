
#ifndef TEMPESTDB_H
#define TEMPESTDB_H

#include "SQLDB.h"

class ActionQueue;
class HighScoreList;

class TempestDB {
public:
   TempestDB(void);
   ~TempestDB(void);

   void  Open(const std::string &path);
   bool LoadHighScores(HighScoreList &);
   void SaveHighScores(HighScoreList highScores);

private:
   SQLDB  db;
   ActionQueue *actionQueue = nullptr;
};

#endif
