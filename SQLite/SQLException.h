
#ifndef SQLEXCEPTION_H
#define SQLEXCEPTION_H

class SQLException : public std::runtime_error {
public:
   SQLException(sqlite3 *db, int error, const std::string &message);

private:
   static std::string FormatMessage(sqlite3 *db, int error, const std::string &message);
   static std::string GetExtendedErrorString(sqlite3 *db);
};


#endif
