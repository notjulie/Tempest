
#include "TempestIO.Headers.h"
#include "HighScoreList.h"

HighScoreList::HighScoreList(void)
{
}

uint32_t HighScoreList::GetScore(int index) const
{
   return highScores[index].score;
}

TempestChar HighScoreList::GetInitial(int index, int charIndex)
{
   return highScores[index].initials[charIndex];
}

void HighScoreList::SetInitial(int index, int charIndex, TempestChar value)
{
   highScores[index].initials[charIndex] = value;
}

void HighScoreList::SetScore(int index, uint32_t score)
{
   highScores[index].score = score;
}

std::string HighScoreList::GetInitials(int index) const
{
   std::string s;
   s += highScores[index].initials[0].ToAscii();
   s += highScores[index].initials[1].ToAscii();
   s += highScores[index].initials[2].ToAscii();
   return s;
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

void HighScoreList::SetInitials(int index, const std::string &initials)
{
   for (int i = 0; i < 3; ++i)
   {
      if (i >= initials.length())
         highScores[i].initials[i] = TempestChar();
      else
         highScores[i].initials[i] = TempestChar::FromAscii(initials[i]);
   }
}

void SetScore(int index, uint32_t score);
