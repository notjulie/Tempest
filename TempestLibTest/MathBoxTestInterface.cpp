
#include "stdafx.h"
#include "MathBox/MathBox.h"
#include "MathBoxTestInterface.h"


Am2901 *MathBoxTestInterface::GetALU(MathBox *mathBox, char which)
{
	switch (which)
	{
	case 'E':
		return &mathBox->aluE;
	case 'F':
		return &mathBox->aluF;
	case 'J':
		return &mathBox->aluJ;
	case 'K':
		return &mathBox->aluK;
	default:
		return NULL;
	}
}
