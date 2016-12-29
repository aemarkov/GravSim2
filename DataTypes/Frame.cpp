#include "Frame.h"

Frame::Frame()
{
	Points = nullptr;
}

Frame::~Frame()
{
	if (Points != nullptr)
		delete[] Points;
}
