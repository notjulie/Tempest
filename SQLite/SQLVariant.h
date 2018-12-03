
#ifndef SQLVARIANT_H
#define SQLVARIANT_H

class SQLVariant {
public:
   SQLVariant(void);
   SQLVariant(sqlite3_stmt *statement, int columnIndex);
   SQLVariant(const SQLVariant &source);
   ~SQLVariant(void);

   operator int(void) const;
   operator std::string(void) const;

   SQLVariant &operator=(const SQLVariant &source);

private:
   sqlite3_value *value = nullptr;
};

#endif
