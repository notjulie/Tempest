
#include "SQLiteCommonHeader.h"

#include "SQLRRException.h"

#include "SQLRRDB.h"


SQLRRDB::SQLRRDB(void)
{
}


SQLRRDB::~SQLRRDB(void)
{
   if (db != nullptr)
   {
      sqlite3_close(db);
      db = nullptr;
   }
}


void SQLRRDB::OpenOrCreate(const std::string &path)
{
   int result = sqlite3_open(path.c_str(), &db);
   if (result != SQLITE_OK)
      throw SQLRRException(db, result, std::string("SQLRRDB::OpenOrCreate: ") + path);
}

