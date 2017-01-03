#include <iostream>
#include <omp.h>

#include "Writer\Writer.h"
#include "Frame.h"
#include "Simulator\GravSim.h"

void PointsToFrame(Particle* points, int particleCount,  DataTypes::Frame & frame, float dt);

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Usage:: MPIGrav.exe <output file>";
		return -1;
	}


	omp_set_num_threads(4);

	// ....... ѕараметры симул€ции ...........
	int count = 1000;
	int stepsCount = 500;
	float dt = 1;

	float pointMass = 1;
	float G = 0.01;
	float minDist = 0.1;
	float radius = 30;
	// .......................................

	double start, duration;

	GravSim sim (count, pointMass, G, minDist, glm::vec3(0, 0, 0), glm::vec3(radius,  radius, radius));
	Writer writer(count, argv[1]);
	DataTypes::Frame frame;
	
	frame.Points = new DataTypes::Point[count];
	frame.Count = count;
	
	int perc10 = stepsCount / 10;

	std::cout << "Gravitation simulator v 2.0\n";
	std::cout << "Points: " << count << "\n";
	std::cout << "Steps: " << stepsCount << "\n\n";

	std::cout << "Point mass: " << pointMass << "\n";
	std::cout << "G constant: " << G << "\n";
	std::cout << "Spawn radius: " << radius << "\n";
	std::cout << "Min dist*: " << minDist << "\n\n";

	start = omp_get_wtime();

	for (int step = 0; step < stepsCount; step++)
	{
		sim.CalcFrameOpenMP(dt);

		Particle* points = sim.GetPoints();
		int count = sim.GetPointsCount();

		PointsToFrame(points, count, frame, dt);
		writer.WriteFrame(frame);

		if (step%perc10 == 0)
			std::cout << step / perc10 * 10 << "%\n";
	}

	duration = omp_get_wtime() - start;

	std::cout << "100%\n\n";
	
	std::cout << "Total duration: " << duration << "\n";
	std::cout << "Press any key...\n";
	std::cin.get();

    return 0;
}

void PointsToFrame(Particle* points, int pointsCount, DataTypes::Frame & frame, float dt)
{
	for (int i = 0; i < pointsCount; i++)
	{
		frame.Points[i].X = points[i].Pos[0];
		frame.Points[i].Y = points[i].Pos[1];
		frame.Points[i].Z = points[i].Pos[2];
	}

	frame.Dt = dt;
	frame.Wtf();
}