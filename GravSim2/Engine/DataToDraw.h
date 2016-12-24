#ifndef __DATATODRAW_H__
#define __DATATODRAW_H__

#include <GLEW\glew.h>

class DataToDraw
{
public:
	GLfloat* Points;
	unsigned int PointsCount;
	unsigned int* Indexes;
	unsigned int IndexesCount;

	bool isDelete = false;

	DataToDraw();
	DataToDraw(GLfloat* points, unsigned int points_count, unsigned int * indexes, unsigned int indexes_count, bool isDelete=true);

	~DataToDraw();
};


#endif