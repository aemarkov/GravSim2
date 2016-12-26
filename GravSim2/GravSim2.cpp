// GravSim2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <SDL2\SDL.h>
#include <GLEW\glew.h>
#include <glm\glm.hpp>

#include "Engine\Engine.h"
#include "Engine\DataToDraw.h"
#include "Engine\FiguresCreator.h"


void callback(DataToDraw &  data, float dt);


DataToDraw CreateFigures();
void CreatePoints(int count);


DataToDraw dataToDraw;
DataToDraw points;

int main(int argc, char *argv[])
{
	CreatePoints(1000000);
	dataToDraw = CreateFigures();

	Engine engine(640, 480, 3, 3, callback);
	engine.SetupStaticData(dataToDraw);
	engine.Start();


	return 0;
}

void callback(DataToDraw & data, float dt)
{
	//data = dataToDraw;
	data = points;
}


DataToDraw CreateFigures()
{
	FiguresCreator creator;

	const float circleRad = 1;
	const float circleHeight = 3;
	const int countInCircle = 10;
	
	creator.DrawCircle(glm::vec3(0, 0, 0), circleRad, 36);
	creator.DrawCircle(glm::vec3(0, 0, 0), 2 * circleRad, 36);
	creator.DrawCircle(glm::vec3(0, 0, 0), 3 * circleRad, 36);

	creator.DrawCircle(glm::vec3(0, circleHeight, 0), circleRad, 36);
	creator.DrawCircle(glm::vec3(0, circleHeight, 0), 2 * circleRad, 36);
	creator.DrawCircle(glm::vec3(0, circleHeight, 0), 3 * circleRad, 36);

	creator.DrawLine(glm::vec3(0, 0, 0), glm::vec3(circleRad * 3, 0, 0));
	creator.DrawLine(glm::vec3(0, 0, 0), glm::vec3(0, circleRad * 3, 0));
	creator.DrawLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, circleRad * 3));

	DataToDraw data;
	creator.GetData(data);
	return data;
}


void CreatePoints(int count)
{
	points.Points = new GLfloat[count*3];
	points.Colors = new GLfloat[count];
	points.PointsCount = count;


	for (int i = 0; i < count*3; i++)
	{
		points.Points[i] = (rand() % 600) / 100.0 - 3;
	}

	for (int i = 0; i < count; i ++)
	{
		points.Colors[i] = 255;
	}
} 