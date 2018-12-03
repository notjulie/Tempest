
#ifndef SQLSTATEMENT_H
#define SQLSTATEMENT_H

class SQLParameterList;
class SQLVariant;

class SQLStatement {
public:
   SQLStatement(sqlite3 *db, const std::string &sql, const SQLParameterList &params);
   ~SQLStatement(void);
   bool MoveNext(void);
   SQLVariant GetColumn(int index);

private:
   sqlite3_stmt *statement = nullptr;
   std::string sql;
};

#endif
