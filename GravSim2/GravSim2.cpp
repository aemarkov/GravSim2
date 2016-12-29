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
#include "Reader\Reader.h"


void callback(DataToDraw &  data, float dt);
DataToDraw CreateFigures();
void CreatePoints(int count);
void FrameToPoints(Frame & frame, DataToDraw & data);

DataToDraw dataToDraw;
DataToDraw points;

Reader reader;
unsigned int PointsCount;
Frame* frame;

unsigned int currentRenderFrame = 0;
unsigned int framesPerRenderFrame = 1;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: GravSim2.exe <input file>";
		return -1;
	}

	try
	{
		PointsCount = reader.Open(argv[1]);
		frame = reader.ReadFrame();
		if (frame == nullptr)
			throw std::string("No frames in file");

	}
	catch (std::string error)
	{
		std::cout << "Exception:\n";
		std::cout << error;
	}
		

	dataToDraw = CreateFigures();
	CreatePoints(PointsCount);
	FrameToPoints(*frame, points);

	Engine engine(640, 480, 3, 3, callback);
	engine.SetupStaticData(dataToDraw);
	engine.Start();


	delete frame;
	return 0;
}

void callback(DataToDraw & data, float dt)
{
 	data = points;

	currentRenderFrame++;

	if (currentRenderFrame == framesPerRenderFrame)
	{
		if (reader.ReadFrame(frame))
		{
			std::cout << frame->Dt << "\n";
			FrameToPoints(*frame, points);
		}

		currentRenderFrame = 0;
	}

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


	/*for (int i = 0; i < count*3; i++)
	{
		points.Points[i] = (rand() % 600) / 100.0 - 3;
	}

	for (int i = 0; i < count; i ++)
	{
		points.Colors[i] = 255;
	}*/
} 

void FrameToPoints(Frame & frame, DataToDraw & data)
{
	unsigned int pointCnt = 0;
	unsigned int colorCnt = 0;

	for (unsigned int i = 0; i < frame.Count; i++)
	{
		data.Points[pointCnt] = frame.Points[i].X;
		data.Points[++pointCnt] = frame.Points[i].Y;
		data.Points[++pointCnt] = frame.Points[i].Z;
		++pointCnt;

		data.Colors[colorCnt++] = COLOR(255,2565,255);
	}
}