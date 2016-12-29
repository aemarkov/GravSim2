#include <iostream>
#include "Writer\Writer.h"
#include "Frame.h"
#include "Simulator\GravSim.h"

void PointsToFrame(Points* points, Frame & frame, float dt);

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Usage:: MPIGrav.exe <output file>";
		return -1;
	}

	int count = 100;
	int stepsCount = 1000;
	float dt = 1;

	GravSim sim (count, glm::vec3(0, 0, 0), glm::vec3(2, 2, 2));
	Writer writer(count, argv[1]);
	Frame frame;
	
	frame.Points = new Point[count];
	frame.Count = count;
	
	for (int step = 0; step < stepsCount; step++)
	{
		sim.CalcFrameSingleThread(dt);
		Points* points = sim.GetPoints();

		PointsToFrame(points, frame, dt);
		writer.WriteFrame(frame);
	}

    return 0;
}

void PointsToFrame(Points* points, Frame & frame, float dt)
{
	for (int i = 0; i < points->Count; i++)
	{
		frame.Points[i].X = points->Pos.X[i];
		frame.Points[i].Y = points->Pos.Y[i];
		frame.Points[i].Z = points->Pos.Z[i];

		//std::cout << frame.Points[i].X << " " << frame.Points[i].Y << " " << frame.Points[i].Z << "\n";
	}

	//std::cout << "\n\n";

	frame.Dt = dt;
}