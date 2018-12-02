
#ifndef SQLDB_H
#define SQLDB_H

#include <vector>
#include "sqlite3.h"
#include "SQLVariant.h"

class SQLDB {
public:
   SQLDB(void);
   ~SQLDB(void);

   void OpenOrCreate(const std::string &path);

   bool DoesTableExist(const std::string &name);

   // query execution
   template <typename... Args> SQLVariant ExecuteScalar(const std::string &sql, Args... args) {
      return DoScalarQuery(sql, SQLVariantList(args...));
   }

private:
   SQLVariant DoScalarQuery(const std::string &sql, const SQLVariantList &params);

private:
   sqlite3  *db = nullptr;
};


#endif
