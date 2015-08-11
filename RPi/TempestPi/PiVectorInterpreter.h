
#ifndef PIVECTORINTERPRETER
#define PIVECTORINTERPRETER

#include "TempestIO/Vector/VectorDataInterpreter.h"

class PiScreen;

class PiVector
{
public:
   PiVector(void);
   PiVector(
      int16_t startX,
      int16_t startY,
      int16_t endX,
      int16_t endY,
      int color
      );

   void Display(PiScreen *screen) const;

private:
	int16_t startX;
	int16_t startY;
	int16_t endX;
	int16_t endY;
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
	bool ClipEndPoint(int &startX, int &startY, int &endX, int &endY);

private:
	int nextIndex;
	std::vector<PiVector> vectors;
	int color;
	int x, y;
	int binaryScale;
	int linearScale;
};

#endif
