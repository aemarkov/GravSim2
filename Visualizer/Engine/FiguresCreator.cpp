#include "FiguresCreator.h"

FiguresCreator::FiguresCreator()
{
}

FiguresCreator::~FiguresCreator()
{
}

void FiguresCreator::DrawCircle(glm::vec3 center, float radius, int count)
{
	GLfloat* points = new GLfloat[3 * count];
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

	indexes[count * 2 - 2] = count - 1;
	indexes[count * 2 - 1] = 0;

	DataToDraw data(points, count, indexes, count * 2);
	addFigure(data);
}

void FiguresCreator::DrawLine(glm::vec3 start, glm::vec3 end)
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

	DataToDraw data(points, 2, indexes, 2);
	addFigure(data);
}

void FiguresCreator::GetData(DataToDraw & data)
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


void FiguresCreator::addFigure(DataToDraw & figure)
{
	int displacement = points.size() / 3;

	for (int i = 0; i < figure.PointsCount * 3; i++)
		points.push_back(figure.Points[i]);

	for (int i = 0; i < figure.IndexesCount; i++)
		indexes.push_back(figure.Indexes[i] + displacement);
}