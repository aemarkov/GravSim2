#include <iostream>
#include <omp.h>
#include <mpi.h>

#include "SimParams\CommandLineParser.h"
#include "MpiCommon.h"
#include "Simulator\MpiGravSim.h"

void Simulate(SimParams & params);

int main(int argc, char** argv)
{
	int processNum;
	SimParams params;


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcessRank);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcessNum);

	//Обработка аргументов командной строки
	if (ProcessRank == 0)
	{
		if (!ParseCommandLine(argc, argv, params, processNum))
		{
			MPI_Finalize();
			return 0;
		}
	}

	MPI_Bcast(&processNum, 1, MPI_INT, RootProc, MPI_COMM_WORLD);

	if (processNum != -1)
		omp_set_num_threads(processNum);

	Simulate(params);

	MPI_Finalize();
	
    return 0;
}



void Simulate(SimParams & params)
{

	double start, duration;
	MpiGravSim sim(params.ParticlesCount, params.PointMass, params.G, params.MinDist, glm::vec3(0, 0, 0), glm::vec3(params.Radius, params.Radius, params.Radius));


	if (ProcessRank == RootProc)
	{
		/*std::cout << "Gravitation simulator v 2.0\n";
		std::cout << "Points: " << params.ParticlesCount << "\n";
		std::cout << "Steps: " << params.StepsCount << "\n\n";

		std::cout << "Point mass: " << params.PointMass << "\n";
		std::cout << "G constant: " << params.G << "\n";
		std::cout << "Spawn radius: " << params.Radius << "\n";
		std::cout << "Min dist*: " << params.MinDist << "\n\n";*/
	}

	start = omp_get_wtime();
	sim.CalcStep(params.Dt);
	duration = omp_get_wtime() - start;

	if(ProcessRank==RootProc)
	{
		/*std::cout << "100%\n\n";

		std::cout << "Total duration: " << duration << "\n";
		std::cout << "Press any key...\n";*/
		//std::cin.get();
	}
}