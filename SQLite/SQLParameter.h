// ====================================================================
// SQLite wrapper
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This defines the SQLParameter class and its kin...the purpose is
//    to simplify passing parameters to queries and to encapsulate the
//    binding of parameters.
// ====================================================================


#ifndef SQLPARAMETER_H
#define SQLPARAMETER_H

#include <memory>
#include <vector>


/// =============================================================================
/// ======================== SQLAbstractParameterBinder =========================
/// =============================================================================

/// <summary>
/// Base class for classes that know how to bind parameters to queries
/// </summary>
class SQLAbstractParameterBinder {
public:
   virtual ~SQLAbstractParameterBinder(void) {}
   virtual SQLAbstractParameterBinder *Clone(void) const = 0;
   virtual int Bind(sqlite3_stmt *statement, int index) const = 0;
};



/// =============================================================================
/// ======================= Parameter binding primitives ========================
/// =============================================================================
// These are intended to be used internally by SQLParameterBinder, but they're
// perfectly generic and there's no reason for them not to be public.

int BindSQLiteParameterToStatement(int value, sqlite3_stmt *statement, int index);
int BindSQLiteParameterToStatement(const std::string &value, sqlite3_stmt *statement, int index);


/// =============================================================================
/// ============================ SQLParameterBinder =============================
/// =============================================================================

/// <summary>
/// Template class for binding values of different types to queries.
/// </summary>
template <typename T> class SQLParameterBinder : public SQLAbstractParameterBinder {
public:
   SQLParameterBinder(const T &t) {
      value = t;
   }

   virtual SQLAbstractParameterBinder *Clone(void) const {
      return new SQLParameterBinder(value);
   }

   virtual int Bind(sqlite3_stmt *statement, int index) const {
      return BindSQLiteParameterToStatement(value, statement, index);
   }

private:
   T value;
};



/// =============================================================================
/// =============================== SQLParameter ================================
/// =============================================================================

/// <summary>
/// Represents a parameter that can be bound to a query.
/// </summary>
class SQLParameter {
public:
   SQLParameter(const SQLParameter &source);
   template <typename T> SQLParameter(const T &t) {
      handler.reset(new SQLParameterBinder<T>(t));
   }

   SQLParameter &operator=(const SQLParameter &source);
   void Bind(sqlite3_stmt *statement, int index) const;

private:
   std::unique_ptr<SQLAbstractParameterBinder> handler;
};


/// =============================================================================
/// ============================= SQLParameterList ==============================
/// =============================================================================

/// <summary>
/// A collection of SQLParameter objects; the variadic template constructor allows
/// a lot of ability to pass variable parameter lists.
/// </summary>
class SQLParameterList : public std::vector<SQLParameter> {
public:
   SQLParameterList(void) {}
   template <typename... Args> SQLParameterList(Args... args) {
      Append(args...);
   }

private:
   template <typename...Args> void Append(const SQLParameter &v, Args...args) {
      push_back(v);
      Append(args...);
   }
   void Append(const SQLParameter &v) { push_back(v); }
};

#endif
