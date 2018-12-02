
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


bool SQLDB::DoesTableExist(const std::string &name)
{
   auto count = (int)ExecuteScalar("SELECT name FROM sqlite_master WHERE type='table' AND name=?1", name);
   if (count == 0)
      return false;
   else if (count == 1)
      return true;
   else
      throw SQLException(std::string("SQLDB::DoesTableExist: multiple results for table ") + name);
}


SQLVariant SQLDB::DoScalarQuery(const std::string &sql, const SQLVariantList &params)
{
   throw SQLException("SQLDB::DoScalarQuery: not implemented");
}
