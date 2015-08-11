
#ifndef PIVECTORINTERPRETER
#define PIVECTORINTERPRETER

#include "TempestIO/Vector/VectorDataInterpreter.h"

class PiScreen;

class PiVector
{
public:
   PiVector(void);
   PiVector(
      float startX,
      float startY,
      float endX,
      float endY,
      int color
      );

   void Display(PiScreen *screen) const;

private:
	float startX;
	float startY;
	float endX;
	float endY;
	int color;
};

class PiVectorInterpreter : public VectorDataInterpreter
{
public:
	PiVectorInterpreter(void);

	void ClearVectors(void) { vectors.resize(0); }
	bool GetNextVector(PiVector &_vector);
	void GetAllVectors(std::vector<PiVector> &vectors) { vectors = this->vectors; }

protected:
	virtual void Center(void);
	virtual void LDraw(int x, int y, int intensity);
	virtual void Scale(int binaryScale, int linearScale);
	virtual void SDraw(int x, int y, int intensity);
	virtual void Stat(int color, int intensity);

private:
	int nextIndex;
	std::vector<PiVector> vectors;
	int color;
	float x, y;
	float scale;
};

#endif

