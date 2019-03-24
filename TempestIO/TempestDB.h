
#ifndef TEMPESTDB_H
#define TEMPESTDB_H

#include "AbstractTempestDB.h"
#include "SQLDB.h"

class ActionQueue;
class HighScoreList;

class TempestDB : public AbstractTempestDB {
public:
   TempestDB(void);
   ~TempestDB(void);

   void  Open(const std::string &path);
   bool LoadHighScores(HighScoreList &);
   void SaveHighScores(const HighScoreList &highScores);

private:
   SQLDB  db;
   ActionQueue *actionQueue = nullptr;
};

#endif
