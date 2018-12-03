
#include "TempestIO.Headers.h"
#include "VectorGameException.h"
#include "TempestDB.h"


void TempestDB::Open(const std::string &path)
{
   // create
   db.OpenOrCreate(path);

   // add the HighScore table if there is none
   db.ExecuteNonQuery(
      "CREATE TABLE IF NOT EXISTS HighScore ("
      "Place INTEGER,"
      "Score INTEGER,"
      "Name TEXT"
      ")"
   );
}

