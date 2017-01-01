#ifndef __FRAME_H__
#define  __FRAME_H__

#include "Point.h"

namespace DataTypes
{

	struct Frame
	{
		double Dt;
		Point* Points;
		unsigned int Count;

		Frame();
		~Frame();
	};
}

#endif