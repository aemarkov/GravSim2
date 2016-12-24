#include "DataToDraw.h"

DataToDraw::DataToDraw()
{
	Points = nullptr;
	Indexes = nullptr;

	PointsCount = 0;
	IndexesCount = 0;
}

DataToDraw::DataToDraw(GLfloat* points, unsigned int points_count, unsigned int * indexes, unsigned int indexes_count)
{
	Points = points;
	PointsCount = points_count;
	Indexes = indexes;
	IndexesCount = indexes_count;
}