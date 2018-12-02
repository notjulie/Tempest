
#include "SQLiteCommonHeader.h"

#include "SQLException.h"

#include "SQLDB.h"


SQLDB::SQLDB(void)
{
}


SQLDB::~SQLDB(void)
{
   if (db != nullptr)
   {
      sqlite3_close(db);
      db = nullptr;
   }
}


void SQLDB::OpenOrCreate(const std::string &path)
{
   int result = sqlite3_open(path.c_str(), &db);
   if (result != SQLITE_OK)
      throw SQLException(db, result, std::string("SQLDB::OpenOrCreate: ") + path);
}


