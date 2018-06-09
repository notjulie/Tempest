
#include "SQLiteCommonHeader.h"
#include "SQLRRException.h"


SQLRRException::SQLRRException(sqlite3 *db, int error, const std::string &message)
   : VectorGameException(FormatMessage(db, error, message))
{
}


std::string SQLRRException::FormatMessage(sqlite3 *db, int error, const std::string &message)
{
   std::ostringstream result;
   result << "SQLite exception, Error #" << error;

   // add the error code
   switch (error)
   {
   case SQLITE_CANTOPEN:
      result << " (SQLITE_CANTOPEN: " << GetExtendedErrorString(db) << ")";
      break;
   }

   return result.str();
}


std::string SQLRRException::GetExtendedErrorString(sqlite3 *db)
{
   int extendedError = sqlite3_extended_errcode(db);

   std::ostringstream result;
   result << "0x" << std::hex << extendedError;
   return result.str();
}
