
#ifndef TEMPESTDB_H
#define TEMPESTDB_H

#include "SQLite/SQLRR/SQLRRDB.h"

class TempestDB {
public:
   void  Open(const std::string &path);

private:
   SQLRRDB  db;
};

#endif
