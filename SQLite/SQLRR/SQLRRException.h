
#ifndef SQLRREXCEPTION_H
#define SQLRREXCEPTION_H

#include "TempestException.h"

class SQLRRException : public TempestException {
public:
   SQLRRException(sqlite3 *db, int error, const std::string &message)
      : TempestException(FormatMessage(db, error, message))
   {
   }

private:
   static std::string FormatMessage(sqlite3 *db, int error, const std::string &message)
   {
      std::string result("SQLite exception");

      char s[100];
      snprintf(s, sizeof(s), ", Error #%d", error);
      result += s;

      // add the error code
      switch (error)
      {
      case SQLITE_CANTOPEN:
         result += " (SQLITE_CANTOPEN: ";
         result += GetExtendedErrorString(db);
         result += ")";
         break;
      }

      return result;
   }

   static std::string GetExtendedErrorString(sqlite3 *db)
   {
      int extendedError = sqlite3_extended_errcode(db);

      char s[100];
      snprintf(s, sizeof(s), "0x%04X", extendedError);
      return s;
   }
};


#endif
