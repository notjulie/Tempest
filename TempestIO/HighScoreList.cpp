
#include "TempestIO.Headers.h"
#include "HighScoreList.h"

HighScoreList::HighScoreList(void)
{
}

uint32_t HighScoreList::GetScore(int index) const
{
   return highScores[index].score;
}

char HighScoreList::GetInitial(int index, int charIndex)
{
   return highScores[index].initials[charIndex];
}

void HighScoreList::SetInitial(int index, int charIndex, char value)
{
   highScores[index].initials[charIndex] = value;
}

uint8_t HighScoreList::InsertScore(uint32_t score)
{
   for (uint8_t i = 0; i < HIGH_SCORE_COUNT; ++i)
   {
      if (score > highScores[i].score)
      {
         // we find were it's supposed to be, so move everything else down
         for (int j = HIGH_SCORE_COUNT - 1; j > i; --j)
            highScores[j] = highScores[j - 1];

         // insert the new score
         highScores[i] = HighScore();
         highScores[i].score = score;

         // and the caller expects a 1-based rank instead of a zero-based index
         return ++i;
      }
   }

   return HIGH_SCORE_COUNT;
}

