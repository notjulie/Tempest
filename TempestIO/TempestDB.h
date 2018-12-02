
#ifndef TEMPESTDB_H
#define TEMPESTDB_H

#include "SQLDB.h"

class HighScoreList;

class TempestDB {
public:
   void  Open(const std::string &path);
   bool LoadHighScores(HighScoreList &) { return false; }
   void SaveHighScores(const HighScoreList &) {}

private:
   SQLDB  db;
};

#endif
