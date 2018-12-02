
#include "SQLiteCommonHeader.h"
#include "SQLException.h"
#include "SQLVariant.h"

SQLVariant::SQLVariant(const std::string &s)
{
   throw SQLException("SQLVariant::SQLVariant: not implemented for string");
}

SQLVariant::operator int(void) const
{
   throw SQLException("SQLVariant::SQLVariant: int cast operator not implemented");
}
