
#ifndef TEMPESTDB_H
#define TEMPESTDB_H

#include "SQLRRDB.h"

class HighScoreList;

class TempestDB {
public:
   void  Open(const std::string &path);
   bool LoadHighScores(HighScoreList &) { return false; }
   void SaveHighScores(const HighScoreList &) {}

private:
   SQLRRDB  db;
};

#endif
