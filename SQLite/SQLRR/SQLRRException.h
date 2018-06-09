
#ifndef SQLRREXCEPTION_H
#define SQLRREXCEPTION_H

#include "VectorGameException.h"

class SQLRRException : public VectorGameException {
public:
   SQLRRException(sqlite3 *db, int error, const std::string &message);

private:
   static std::string FormatMessage(sqlite3 *db, int error, const std::string &message);
   static std::string GetExtendedErrorString(sqlite3 *db);
};


#endif
