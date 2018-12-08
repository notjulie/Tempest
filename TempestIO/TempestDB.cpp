
#include "TempestIO.Headers.h"

#include "ActionQueue.h"
#include "HighScoreList.h"
#include "SQLStatement.h"
#include "VectorGameException.h"

#include "TempestDB.h"

TempestDB::TempestDB(void)
{
   actionQueue = ActionQueue::Create();
}

TempestDB::~TempestDB(void)
{
   delete actionQueue;
}

void TempestDB::Open(const std::string &path)
{
   actionQueue->ExecuteSynchronous<void>([this,path]() {
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
   });
}

bool TempestDB::LoadHighScores(HighScoreList &highScores)
{
   return actionQueue->ExecuteSynchronous<bool>([&]() {
      SQLStatement statement = db.ExecuteQuery("SELECT Place,Score,Name FROM HighScore");

      HighScoreList result;
      int count = 0;
      while (statement.MoveNext())
      {
         int place = (int)statement.GetColumn(0);
         result.SetScore(place - 1, (int)statement.GetColumn(1));
         result.SetInitials(place - 1, (std::string)statement.GetColumn(2));
         ++count;
      }

      return count == HIGH_SCORE_COUNT;
   });
}

void TempestDB::SaveHighScores(HighScoreList highScores)
{
   actionQueue->ExecuteAsynchronous([highScores,this]() {
      db.ExecuteNonQuery("BEGIN TRANSACTION");

      try {
         // delete the high scores
         db.ExecuteNonQuery("DELETE FROM HighScore");

         // and insert the new ones
         for (int i = 0; i < HIGH_SCORE_COUNT; ++i)
            db.ExecuteNonQuery(
               "INSERT INTO HighScore (Place,Score,Name) VALUES (?1, ?2, ?3)",
               i + 1,
               highScores.GetScore(i),
               highScores.GetInitials(i)
            );

         // commit
         db.ExecuteNonQuery("COMMIT TRANSACTION");
      }
      catch (...) {
         db.ExecuteNonQuery("ROLLBACK TRANSACTION");
      }
   });
}
