#include "CommandLineParser.h"

void ShowUsage();
bool LoadParamsFile(const char* filename, SimParams& params);

/*
 * Аргументы вида:
 *    binary  -particles <num of particles> -steps <num of steps> -params <simulation params file>  -out <output file> [-nt <number of threads>]
 *
 * Вид файла параметров:
 *   <Dt>
 *   <Point mass>
 *   <G>
 *   <Min dist>
 *   <Radius>
 *
 */
bool ParseCommandLine(int argc, char ** argv, CmdParams & params)
{
	if (argc != 9 && argc != 11)
	{
		std::cout << "Invalid number of arguments\n";
		ShowUsage();
		return false;
	}

	int currentParam = 1;

	int numOfParticles;           bool isNumOfParticlesEntered = false;
	int steps;				      bool isStepsEntered = false;
	char* paramFile;              bool isParamFileEntered = false;
	char* outputFile;             bool isOutputFileEntered = false;
	params.NumberOfThreads = -1;


	//Парсим аргументы
	while (currentParam < argc)
	{
		if (strcmp(argv[currentParam], "-particles") == 0)
		{
			currentParam++;
			numOfParticles = atoi(argv[currentParam]);
			isNumOfParticlesEntered = true;
		}
		else if (strcmp(argv[currentParam], "-steps") == 0)
		{
			currentParam++;
			steps = atoi(argv[currentParam]);
			isStepsEntered = true;
		}
		else if (strcmp(argv[currentParam], "-params") == 0)
		{
			currentParam++;
			paramFile = argv[currentParam];
			isParamFileEntered = true;
		}
		else if (strcmp(argv[currentParam], "-out") == 0)
		{
			currentParam++;
			outputFile = argv[currentParam];
			isOutputFileEntered = true;
		}
		else if (strcmp(argv[currentParam], "-nt") == 0)
		{
			currentParam++;
			params.NumberOfThreads = atoi(argv[currentParam]);
		}
		else
		{
			std::cout << "Invalid argument " << argv[currentParam] << "\n";
			ShowUsage();
			return false;
		}

		currentParam++;
	}

	if (!isNumOfParticlesEntered || !isStepsEntered || !isParamFileEntered || !isOutputFileEntered)
	{
		std::cout << "Not enough arguments\n";
		ShowUsage();
		return false;
	}
	
	params.ParticlesCount = numOfParticles;
	params.StepsCount = steps;
	params.OutputFile = outputFile;
	params.ParamsFile = paramFile;

	return true;
}

void ShowUsage()
{
	std::cout << "Usage: \n";
	std::cout << "binary  -particles <num of particles> -steps <num of steps> -params <simulation params file>  -out <output file> [-nt <number of threads>]\n";
}


//Читает параметры из файла конфигурации
bool LoadParamsFile(const char* filename, SimParams& params)
{
	std::ifstream stream;
	stream.open(filename);

	if (!stream)
	{
		std::cout << "Can't open parameters file " << filename << "\n";
		return false;
	}

	stream >> params.Dt;
	stream >> params.PointMass;
	stream >> params.G;
	stream >> params.MinDist;
	stream >> params.Radius;

	return true;

}