
#ifndef SQLSTATEMENT_H
#define SQLSTATEMENT_H

struct sqlite3;
struct sqlite3_stmt;
class SQLParameterList;
class SQLVariant;

class SQLStatement {
public:
   SQLStatement(sqlite3 *db, const std::string &sql, const SQLParameterList &params);
   SQLStatement(const SQLStatement &) = delete;
   SQLStatement(const SQLStatement &&source);
   ~SQLStatement(void);
   SQLStatement &operator=(const SQLStatement &) = delete;

   bool MoveNext(void);
   SQLVariant GetColumn(int index);

private:
   sqlite3_stmt *statement = nullptr;
   std::string sql;
};

#endif
