#include <iostream>
#include <omp.h>

#include "SimParams\CommandLineParser.h"
#include "Writer\Writer.h"
#include "Frame.h"
#include "Simulator\GravSim.h"

void PointsToFrame(Particle* points, int particleCount,  DataTypes::Frame & frame, float dt);
void Simulate(SimParams & params, const char* filename);

int main(int argc, char** argv)
{
	SimParams simParams;
	CmdParams cmdParams;

	if (!ParseCommandLine(argc, argv, cmdParams))
		return 0;

	if (!LoadParamsFile(cmdParams.ParamsFile, simParams))
		return 0;

	simParams.ParticlesCount = cmdParams.ParticlesCount;
	simParams.StepsCount = cmdParams.StepsCount;

	if(cmdParams.NumberOfThreads!=-1)
		omp_set_num_threads(cmdParams.NumberOfThreads);	


	Simulate(simParams, cmdParams.OutputFile);


    return 0;
}

void Simulate(SimParams & params, const char* filename)
{
	double start, duration;

	GravSim sim(params.ParticlesCount, params.PointMass, params.G, params.MinDist, glm::vec3(0, 0, 0), glm::vec3(params.Radius, params.Radius, params.Radius));
	Writer writer(params.ParticlesCount, filename);
	DataTypes::Frame frame;

	frame.Points = new DataTypes::Point[params.ParticlesCount];
	frame.Count = params.ParticlesCount;

	int perc10 = params.StepsCount / 10;

	std::cout << "Gravitation simulator v 2.0\n";
	std::cout << "Points: " << params.ParticlesCount << "\n";
	std::cout << "Steps: " << params.StepsCount << "\n\n";

	std::cout << "Point mass: " << params.PointMass << "\n";
	std::cout << "G constant: " << params.G << "\n";
	std::cout << "Spawn radius: " << params.Radius << "\n";
	std::cout << "Min dist*: " << params.MinDist << "\n\n";

	start = omp_get_wtime();

	for (int step = 0; step < params.StepsCount; step++)
	{
		sim.CalcFrameOpenMP(params.Dt);

		Particle* points = sim.GetPoints();
		int count = sim.GetPointsCount();

		PointsToFrame(points, count, frame, params.Dt);
		writer.WriteFrame(frame);

		if (perc10 != 0 &&  step%perc10 == 0)
			std::cout << step / perc10 * 10 << "%\n";
	}

	duration = omp_get_wtime() - start;

	std::cout << "100%\n\n";

	std::cout << "Total duration: " << duration << "\n";
	std::cout << "Press any key...\n";
	std::cin.get();
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
}