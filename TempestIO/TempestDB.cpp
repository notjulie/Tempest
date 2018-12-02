
#include "TempestIO.Headers.h"
#include "VectorGameException.h"
#include "TempestDB.h"


void TempestDB::Open(const std::string &path)
{
   db.OpenOrCreate(path);
   //if (!db.DoesTableExist("HighScore"))
      //throw VectorGameException("TempestDB::Open: missing table");
}

