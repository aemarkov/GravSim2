// GravSim2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <SDL2\SDL.h>
#include <GLEW\glew.h>

#include "Engine\Engine.h"
#include "Engine\DataToDraw.h"

void callback(DataToDraw & data, float dt);

int main(int argc, char *argv[])
{
	Engine engine(640, 480, 3,3, callback);
    return 0;
}

void callback(DataToDraw & data, float dt)
{
	GLfloat points[] =
	{
		-1, -1, 7,
		-1, 1,  7,
		1, 1,  7,
		1, -1, 7
	};

	/*unsigned int indexes[] =
	{
	0, 1, 2,
	0, 2, 3,
	};*/

	unsigned int indexes[] =
	{
		0, 1, 1, 2, 2, 0,
		0, 2, 2, 3, 3, 0
	};

	data.Indexes = indexes;
	data.IndexesCount = 12;
	data.Points = points;
	data.PointsCount = 4;
}
