
#include "SQLiteCommonHeader.h"
#include "SQLException.h"
#include "SQLVariant.h"


SQLVariant::SQLVariant(void)
{
}

SQLVariant::SQLVariant(sqlite3_stmt *statement, int columnIndex)
{
   // get the value... this is an "unprotected" value that points to the
   // statement's internal memory, so we don't own
   sqlite3_value *unprotectedValue = sqlite3_column_value(statement, columnIndex);

   // duplicate it, which gives us a protected value
   if (unprotectedValue != nullptr)
      this->value = sqlite3_value_dup(unprotectedValue);
}

SQLVariant::SQLVariant(const SQLVariant &source)
{
   operator=(source);
}

SQLVariant::~SQLVariant(void)
{
   if (value != nullptr)
      sqlite3_value_free(value);
}

SQLVariant::operator int(void) const 
{
   if (value == nullptr)
      throw SQLException("SQLVariant::int: value is NULL");
   return sqlite3_value_int(value);
}

SQLVariant::operator std::string(void) const
{
   if (value == nullptr)
      throw SQLException("SQLVariant::int: value is NULL");
   return (const char *)sqlite3_value_text(value);
}

SQLVariant &SQLVariant::operator=(const SQLVariant &source)
{
   // free the current value
   if (value != nullptr)
   {
      sqlite3_value_free(value);
      value = nullptr;
   }
   
   // duplicate the given value
   if (source.value != nullptr)
      value = sqlite3_value_dup(source.value);
   
   // done
   return *this;
}

