
#ifndef SQLDB_H
#define SQLDB_H

#include "sqlite3.h"

class SQLDB {
public:
   SQLDB(void);
   ~SQLDB(void);

   void OpenOrCreate(const std::string &path);

private:
   sqlite3  *db = nullptr;
};


#endif
