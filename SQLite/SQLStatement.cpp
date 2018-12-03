
#include "SQLiteCommonHeader.h"
#include "SQLException.h"
#include "SQLParameter.h"
#include "SQLVariant.h"
#include "SQLStatement.h"


SQLStatement::SQLStatement(sqlite3 *db, const std::string &sql, const SQLParameterList &params)
{
   // copy parameters
   this->sql = sql;

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
}


SQLStatement::~SQLStatement(void)
{
   sqlite3_finalize(statement);
}

bool SQLStatement::MoveNext(void)
{
   // execute once
   int sqlResult = sqlite3_step(statement);

   // get the result
   switch (sqlResult)
   {
   case SQLITE_DONE:
      // no rows left
      return false;

   case SQLITE_ROW:
      // got data
      return true;

   default:
      // error
      throw SQLException(sqlite3_db_handle(statement), sqlResult, std::string("sqlite3_prepare_v2 error in query: ") + sql);
   }
}

SQLVariant SQLStatement::GetColumn(int index)
{
   return SQLVariant(statement, index);
}

