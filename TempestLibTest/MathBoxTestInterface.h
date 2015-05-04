
#ifndef MATHBOXTESTINTERFACE_H
#define MATHBOXTESTINTERFACE_H

class Am2901;
class MathBox;

class MathBoxTestInterface
{
public:
	static Am2901 *GetALU(MathBox *mathBox, char which);
};

#endif
