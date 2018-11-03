
#include "TempestIO.Headers.h"
#include "TempestDB.h"


void  TempestDB::Open(const std::string &path)
{
   db.OpenOrCreate(path);
}

