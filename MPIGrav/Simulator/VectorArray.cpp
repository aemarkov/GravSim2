#include "VectorArray.h"

VectorArray::~VectorArray()
{
	if (X != nullptr)
		delete[] X;

	if (Y != nullptr)
		delete[] Y;

	if (Z != nullptr)
		delete[] Z;
}
