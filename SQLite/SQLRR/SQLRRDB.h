
#ifndef SQLRRDB_H
#define SQLRRDB_H

#include "sqlite3.h"

class SQLRRDB {
public:
   SQLRRDB(void);
   ~SQLRRDB(void);

   void OpenOrCreate(const std::string &path);

private:
   sqlite3  *db = nullptr;
};


#endif
