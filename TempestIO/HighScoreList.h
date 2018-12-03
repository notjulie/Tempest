
#ifndef HIGHSCORELIST_H
#define HIGHSCORELIST_H

#include "TempestChar.h"

const uint8_t HIGH_SCORE_COUNT = 100;

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

struct HighScore {
   TempestChar initials[3];
   uint32_t score = 10101;
};

class HighScoreList {
public:
   HighScoreList(void);

   uint32_t GetScore(int index) const;
   TempestChar GetInitial(int index, int charIndex);
   std::string GetInitials(int index) const;

   uint8_t   InsertScore(uint32_t score);

   void SetInitial(int index, int charIndex, TempestChar value);
   void SetInitials(int index, const std::string &initials);
   void SetScore(int index, uint32_t score);

private:
   HighScore highScores[HIGH_SCORE_COUNT];
};


#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
