
#include "SQLiteCommonHeader.h"

#include "SQLException.h"
#include "SQLStatement.h"

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
   auto count = (int)ExecuteScalar("SELECT COUNT(name) FROM sqlite_master WHERE type='table' AND name=?1", name);
   if (count == 0)
      return false;
   else if (count == 1)
      return true;
   else
      throw SQLException(std::string("SQLDB::DoesTableExist: multiple results for table ") + name);
}


SQLVariant SQLDB::DoScalarQuery(const std::string &sql, const SQLParameterList &params)
{
   SQLStatement statement(db, sql, params);

   if (!statement.MoveNext())
      return SQLVariant();

   return statement.GetColumn(0);
}


void SQLDB::DoNonQuery(const std::string &sql, const SQLParameterList &params)
{
   // create the statement
   SQLStatement statement(db, sql, params);

   // this actually executes the query, so we need to call it even though
   // we aren't looking for results
   statement.MoveNext();
}
