
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
   sqlite3_stmt *statement = nullptr;

   try {
      // prepare the statement
      int sqlResult = sqlite3_prepare_v2(
         db,
         sql.c_str(),
         (int)sql.length(),
         &statement,
         nullptr
      );
      if (sqlResult != SQLITE_OK)
         throw SQLException(db, sqlResult, std::string("sqlite3_prepare_v2 error in query: ") + sql);

      // bind the parameters
      for (int i = 0; i < params.size(); ++i)
         params[i].Bind(statement, i + 1);

      // execute once
      sqlResult = sqlite3_step(statement);

      // get the result
      SQLVariant result;
      switch (sqlResult)
      {
      case SQLITE_DONE:
         // no rows returned... leave the result as null
         break;

      case SQLITE_ROW:
         // got data... return the value from column zero
         result = SQLVariant(statement, 0);
         break;

      default:
         // error
         throw SQLException(db, sqlResult, std::string("sqlite3_prepare_v2 error in query: ") + sql);
      }


      // dispose the statement
      if (statement != nullptr)
         sqlite3_finalize(statement);

      // done
      return result;
   }
   catch (...) {
      // clean up and rethrow
      if (statement != nullptr)
         sqlite3_finalize(statement);
      throw;
   }
}
