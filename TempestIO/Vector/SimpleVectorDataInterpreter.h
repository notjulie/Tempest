
#ifndef SIMPLEVECTORDATAINTERPRETER_H
#define SIMPLEVECTORDATAINTERPRETER_H

#include "SimpleVector.h"
#include "VectorDataInterpreter.h"


class SimpleVectorDataInterpreter : public VectorDataInterpreter
{
public:
	SimpleVectorDataInterpreter(void);

	void ClearVectors(void) { vectors.resize(0); }
	bool GetNextVector(SimpleVector &_vector);
	void GetAllVectors(std::vector<SimpleVector> &vectors) { vectors = this->vectors; }

protected:
	virtual void Center(void);
	virtual void LDraw(int x, int y, int intensity);
	virtual void Scale(int binaryScale, int linearScale);
	virtual void SDraw(int x, int y, int intensity);
	virtual void Stat(int color, int intensity);

private:
   void Char(char c);
	bool ClipEndPoint(int &startX, int &startY, int &endX, int &endY);

private:
	int nextIndex;
	std::vector<SimpleVector> vectors;
	int color;
	float x, y;
	int binaryScale;
	int linearScale;
};


#endif
