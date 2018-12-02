
#include "SQLiteCommonHeader.h"
#include "SQLException.h"
#include "SQLParameter.h"


SQLParameter::SQLParameter(const SQLParameter &source)
{
   throw SQLException("SQLParameter: copy constructor not implemented");
}
