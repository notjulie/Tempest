
#ifndef SQLPARAMETER_H
#define SQLPARAMETER_H

#include <vector>

class SQLAbstractParameterBinder {
public:
   virtual ~SQLAbstractParameterBinder(void) {}
};

template <typename T> class SQLParameterBinder : public SQLAbstractParameterBinder {
public:
   SQLParameterBinder(const T &t) {
      value = t;
   }

private:
   T value;
};

class SQLParameter {
public:
   SQLParameter(const SQLParameter &source);
   template <typename T> SQLParameter(const T &t) {
      handler.reset(new SQLParameterBinder<T>(t));
   }

   SQLParameter &operator=(const SQLParameter &source);

private:
   std::unique_ptr<SQLAbstractParameterBinder> handler;
};

class SQLParameterList {
public:
   template <typename... Args> SQLParameterList(Args... args) {
      Append(args...);
   }

private:
   template <typename...Args> void Append(const SQLParameter &v, Args...args) {
      list.push_back(v);
      Append(args...);
   }
   void Append(const SQLParameter &v) { list.push_back(v); }

private:
   std::vector<SQLParameter> list;
};

#endif
