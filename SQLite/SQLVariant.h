
#ifndef SQLVARIANT_H
#define SQLVARIANT_H

#include <vector>

class SQLVariant {
public:
   SQLVariant(const std::string &s);

   operator int(void) const;
};

class SQLVariantList {
public:
   template <typename... Args> SQLVariantList(Args... args) {
      Append(args...);
   }

private:
   template <typename...Args> void Append(const SQLVariant &v, Args...args) {
      list.push_back(v);
      Append(args...);
   }
   void Append(const SQLVariant &v) { list.push_back(v); }

private:
   std::vector<SQLVariant> list;
};

#endif
