
#ifndef HIGHSCORELIST_H
#define HIGHSCORELIST_H

const uint8_t HIGH_SCORE_COUNT = 100;

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

struct HighScore {
   char initials[3] = {0,0,0};
   uint32_t score = 10101;
};

class HighScoreList {
public:
   HighScoreList(void);
   uint32_t GetScore(int index) const;
   uint8_t   InsertScore(uint32_t score);
   char GetInitial(int index, int charIndex);
   void SetInitial(int index, int charIndex, char value);

private:
   HighScore highScores[HIGH_SCORE_COUNT];
};


#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
