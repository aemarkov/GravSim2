#include <iostream>
#include <omp.h>
#include <mpi.h>

#include "SimParams\CommandLineParser.h"
#include "MpiCommon.h"
#include "Simulator\MpiGravSim.h"
#include "Frame.h"
#include "Writer\Writer.h"

void Simulate(SimParams & params, const char* outputFile);
void PointsToFrame(Points& points, DataTypes::Frame & frame, float dt);

int main(int argc, char** argv)
{
	SimParams simParams;
	CmdParams cmdParams;
	int providedLevel;


	MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &providedLevel);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcessRank);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcessNum);

	//Чтение аргументов командной строки
	if (!ParseCommandLine(argc, argv, cmdParams))
	{
		std::cout << "[" << ProcessRank << "] Command line error\n";
		std::cout.flush();

		MPI_Finalize();
		return 0;
	}

	//Чтение параметров симуляции из файла
	if (ProcessRank == RootProc)
	{
		if (!LoadParamsFile(cmdParams.ParamsFile, simParams))
		{
			std::cout << "[" << ProcessRank << "] Params file error\n";
			std::cout.flush();

			MPI_Finalize();
			return 0;
		}

		simParams.ParticlesCount = cmdParams.ParticlesCount;
		simParams.StepsCount = cmdParams.StepsCount;
	}

	//Передача параметров
	MPI_Bcast(&simParams.ParticlesCount, 1, MPI_INT, RootProc, MPI_COMM_WORLD);
	MPI_Bcast(&simParams.StepsCount, 1, MPI_INT, RootProc, MPI_COMM_WORLD);
	MPI_Bcast(&simParams.PointMass, 1, MPI_FLOAT, RootProc, MPI_COMM_WORLD);
	MPI_Bcast(&simParams.G, 1, MPI_FLOAT, RootProc, MPI_COMM_WORLD);
	MPI_Bcast(&simParams.MinDist, 1, MPI_FLOAT, RootProc, MPI_COMM_WORLD);
	MPI_Bcast(&simParams.Radius, 1, MPI_FLOAT, RootProc, MPI_COMM_WORLD);

	if (cmdParams.NumberOfThreads != -1)
		omp_set_num_threads(cmdParams.NumberOfThreads);

	Simulate(simParams, cmdParams.OutputFile);

	MPI_Finalize();

	return 0;
}



void Simulate(SimParams & params, const char* outputFile)
{

	double start, duration;
	MpiGravSim sim(params.ParticlesCount, params.PointMass, params.G, params.MinDist, glm::vec3(0, 0, 0), glm::vec3(params.Radius, params.Radius, params.Radius));
	int perc10 = params.StepsCount / 10;

	Writer writer;
	DataTypes::Frame frame;

	if (ProcessRank == WriterProc)
	{
		writer.Open(params.ParticlesCount, outputFile);
		frame.Points = new DataTypes::Point[params.ParticlesCount];
		frame.Count = params.ParticlesCount;
	}


	if (ProcessRank == RootProc)
	{
		std::cout << "Gravitation simulator v 2.0\n";
		std::cout << "Points: " << params.ParticlesCount << "\n";
		std::cout << "Steps: " << params.StepsCount << "\n\n";

		std::cout << "Point mass: " << params.PointMass << "\n";
		std::cout << "G constant: " << params.G << "\n";
		std::cout << "Spawn radius: " << params.Radius << "\n";
		std::cout << "Min dist*: " << params.MinDist << "\n\n";
	}

	start = omp_get_wtime();


	for (int step = 0; step < params.StepsCount; step++)
	{
		sim.CalcStep(params.Dt);

		if (ProcessRank == WriterProc)
		{
			Points* points = sim.GetPoints();
			PointsToFrame(*points, frame, params.Dt);
			writer.WriteFrame(frame);
		}
	}

	duration = omp_get_wtime() - start;

	if (ProcessRank == RootProc)
	{
		std::cout << "100%\n\n";

		std::cout << "Total duration: " << duration << "\n";
		std::cout << "Press any key...\n";
		//std::cin.get();
	}
}

void PointsToFrame(Points & points, DataTypes::Frame & frame, float dt)
{
	for (int i = 0; i < points.count * 3; i += 3)
	{
		frame.Points[i / 3].X = points.pos[i];
		frame.Points[i / 3].Y = points.pos[i + 1];
		frame.Points[i / 3].Z = points.pos[i + 2];
	}

	frame.Dt = dt;
}
