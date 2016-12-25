// GravSim2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <SDL2\SDL.h>
#include <GLEW\glew.h>
#include <glm\glm.hpp>

#include "Engine\Engine.h"
#include "Engine\DataToDraw.h"

void callback(DataToDraw &  data, float dt);


DataToDraw CreateFigures();

void AddFigure(std::vector<GLfloat> & points, std::vector<unsigned int> & indexes, DataToDraw & figure);
void VectorsToData(DataToDraw& data, std::vector<GLfloat> & points, std::vector<unsigned int> & indexes);

DataToDraw DrawCircle(glm::vec3 center, float radius, int count);
DataToDraw DrawLine(glm::vec3 start, glm::vec3 end);

DataToDraw dataToDraw;

int main(int argc, char *argv[])
{
	dataToDraw = CreateFigures();

	Engine engine(640, 480, 3, 3, callback);
	return 0;
}

void callback(DataToDraw & data, float dt)
{
	

	/*AddFigure(points, indexes, DrawCircle(glm::vec3(-1, 0, 0), 1, 36));
	AddFigure(points, indexes, DrawCircle(glm::vec3(1, 0, 0), 1, 36));

	AddFigure(points, indexes, DrawLine(glm::vec3(-1, 0, 1), glm::vec3(-1, 0, 6)));
	AddFigure(points, indexes, DrawLine(glm::vec3(1, 0, 1), glm::vec3(1, 0, 6)));
	AddFigure(points, indexes, DrawLine(glm::vec3(-1, 0, 6), glm::vec3(1, 0, 6)));*/

	data = dataToDraw;
}


DataToDraw CreateFigures()
{
	const float circleRad = 1;
	const float circleHeight = 3;

	const int countInCircle = 10;
	std::vector<GLfloat> points;
	std::vector<unsigned int> indexes;

	AddFigure(points, indexes, DrawCircle(glm::vec3(0, 0, 0), circleRad, 36));
	AddFigure(points, indexes, DrawCircle(glm::vec3(0, 0, 0), 2 * circleRad, 36));
	AddFigure(points, indexes, DrawCircle(glm::vec3(0, 0, 0), 3 * circleRad, 36));

	AddFigure(points, indexes, DrawCircle(glm::vec3(0, circleHeight, 0), circleRad, 36));
	AddFigure(points, indexes, DrawCircle(glm::vec3(0, circleHeight, 0), 2 * circleRad, 36));
	AddFigure(points, indexes, DrawCircle(glm::vec3(0, circleHeight, 0), 3 * circleRad, 36));

	AddFigure(points, indexes, DrawLine(glm::vec3(0, 0, 0), glm::vec3(circleRad * 3, 0, 0)));
	AddFigure(points, indexes, DrawLine(glm::vec3(0, 0, 0), glm::vec3(0, circleRad * 3, 0)));
	AddFigure(points, indexes, DrawLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, circleRad * 3)));

	DataToDraw data;
	VectorsToData(data, points, indexes);
	return data;
}


void AddFigure(std::vector<GLfloat> & points,  std::vector<unsigned int> & indexes, DataToDraw & figure)
{
	int displacement = points.size()/3;

	for (int i = 0; i < figure.PointsCount * 3; i++)
		points.push_back(figure.Points[i]);

	for (int i = 0; i < figure.IndexesCount; i++)
		indexes.push_back(figure.Indexes[i] + displacement);
}

void VectorsToData(DataToDraw& data, std::vector<GLfloat> & points, std::vector<unsigned int> & indexes)
{
	if (data.Points != NULL)
		delete[] data.Points;

	if (data.IndexesCount != NULL)
		delete[] data.Indexes;

	data.Points = new GLfloat[points.size()];
	data.Indexes = new unsigned int[indexes.size()];

	for (int i = 0; i < points.size(); i++)
		data.Points[i] = points[i];

	for (int i = 0; i < indexes.size(); i++)
		data.Indexes[i] = indexes[i];

	data.PointsCount = points.size() / 3;
	data.IndexesCount = indexes.size();
}

DataToDraw DrawCircle(glm::vec3 center, float radius, int count)
{
	GLfloat* points = new GLfloat[3*count];
	unsigned int * indexes = new unsigned int[2 * count];

	float step = 2.0 * M_PI / count;
	int pointIndex = 0;
	int indexIndex = 0;

	for (int i = 0; i < count; i++)
	{
		float angle = i * step;


		float x = center.x + cos(angle)*radius;
		float z = center.z + sin(angle)*radius;

		points[pointIndex] = x;
		points[++pointIndex] = center.y;
		points[++pointIndex] = z;
		pointIndex++;

		indexes[indexIndex] = i;
		indexes[++indexIndex] = i + 1;
		indexIndex++;
	}

	indexes[count*2 - 2] = count - 1;
	indexes[count*2 - 1] = 0;

	return DataToDraw(points, count, indexes, count * 2);
}


DataToDraw DrawLine(glm::vec3 start, glm::vec3 end)
{
	GLfloat* points = new GLfloat[3 * 2];
	unsigned int * indexes = new unsigned int[2];

	points[0] = start.x;
	points[1] = start.y;
	points[2] = start.z;

	points[3] = end.x;
	points[4] = end.y;
	points[5] = end.z;

	indexes[0] = 0;
	indexes[1] = 1;

	return DataToDraw(points, 2, indexes, 2);
}