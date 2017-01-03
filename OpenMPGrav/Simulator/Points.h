#ifndef __POINTS_H__
#define __POINTS_H__

#include "VectorArray.h"

struct Points
{
	float* Mass;
	VectorArray Pos;
	VectorArray Speed;
	int Count;
};

#endif