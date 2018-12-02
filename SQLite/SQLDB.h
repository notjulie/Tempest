
#ifndef SQLDB_H
#define SQLDB_H

#include <vector>
#include "sqlite3.h"
#include "SQLException.h"
#include "SQLParameter.h"

class SQLVariant {
public:
   operator int(void) const { throw SQLException("SQLVariant::in cast not implemented"); }
};

class SQLDB {
public:
   SQLDB(void);
   ~SQLDB(void);

   void OpenOrCreate(const std::string &path);

   bool DoesTableExist(const std::string &name);

   // query execution
   template <typename... Args> SQLVariant ExecuteScalar(const std::string &sql, Args... args) {
      return DoScalarQuery(sql, SQLParameterList(args...));
   }

private:
   SQLVariant DoScalarQuery(const std::string &sql, const SQLParameterList &params);

private:
   sqlite3  *db = nullptr;
};


#endif
