#include "MpiGravSim.h"

MpiGravSim::MpiGravSim()
{
}

MpiGravSim::MpiGravSim(int count, float pointMass, float G, float minDist, glm::vec3 center, glm::vec3 radius)
{
	Init(count, pointMass, G, minDist, center, radius);
}

MpiGravSim::~MpiGravSim()
{
	if (ProcessRank != RootProc)
	{
		delete[] points.mass;
		delete[] partForces;
		delete[] points.forces;
		delete[] points.speed;
	}
	else
		delete[] tasks;

	delete[] points.pos;
}

void MpiGravSim::Init(int count, float pointMass, float G, float minDist, glm::vec3 center, glm::vec3 radius)
{
	//Создание коммуникатора только для рабочих процессов
	if (ProcessRank == RootProc)
		MPI_Comm_split(MPI_COMM_WORLD, 0, ProcessRank, &workersComm);
	else
		MPI_Comm_split(MPI_COMM_WORLD, 1, ProcessRank, &workersComm);

	MPI_Comm_rank(workersComm, &workersProcessRank);
	MPI_Comm_size(workersComm, &workersProcessNum);


	points.count = count;
	this->G = G;
	this->minDist = minDist;

	//Выделение буферов и инициализация
	points.pos = new float[points.count * 3];

	if (ProcessRank != RootProc)
	{
		points.mass = new float[points.count];
		points.forces = new float[points.count * 3];
		points.speed = new float[points.count * 3];
		partForces = new float[points.count * 3];

		for (int i = 0; i < points.count; i++)
			points.mass[i] = pointMass;

		for (int i = 0; i < points.count * 3; i++)
		{
			points.forces[i] = 0;
			points.speed[i] = 0;
			partForces[i] = 0;
		}
	}

	//Генерация положений
	if (ProcessRank == RootProc)
	{
		for (int i = 0; i < points.count * 3; i += 3)
		{
			points.pos[i + 0] = (float)rand() / RAND_MAX * 2 * radius.x - radius.x + center.x;
			points.pos[i + 1] = (float)rand() / RAND_MAX * 2 * radius.y - radius.y + center.y;
			points.pos[i + 2] = (float)rand() / RAND_MAX * 2 * radius.z - radius.y + center.z;
		}
	}
	
	//Отправка положений
	MPI_Bcast(points.pos, points.count * 3, MPI_FLOAT, RootProc, MPI_COMM_WORLD);

	//Расчитываем число точек на процесс и смещение для каждого процесса
	if (ProcessRank != RootProc)
	{
		pointsPerProcess = points.count / workersProcessNum;
		pointsDisplacement = pointsPerProcess * workersProcessRank;

		if (points.count % workersProcessNum != 0)
		{
			std::cout << "WARNING: points per process not equal for all processes\n";
			std::cout.flush();
		}
	}

	
	//Создание задач (0,0), (0,1) ... (0,n-1), (1,1), (1, 2), ... (1,n-1), ..., (n-1, n-1)
	if (ProcessRank == RootProc)
	{
		int workers = ProcessNum - 1;

		numTasks = workers*(workers + 1) / 2 + workers;

		tasks = new int[numTasks * 2];
		int taskCnt = 0;
		
		//Задачи
		for (int i = 0; i < workers; i++)
		{
			for (int j = i; j < workers; j++)
			{
				tasks[taskCnt++] = i;
				tasks[taskCnt++] = j;
			}
		}

		//Остановки (-1, -1)
		for (int i = 0; i < workers; i++)
		{
			tasks[taskCnt++] = -1;
			tasks[taskCnt++] = -1;
		}

		/*for (int i = 0; i < numTasks*2; i+=2)
		{
			std::cout << "(" << tasks[i] << ", " << tasks[i + 1] << ")\n";
		}*/
	}
}

//Расчет шага симуляции
/*

Принцип работы

Все точки делятся на блоки равного размера (ограничение).
Блоки пронумерованы 0 ... n-1
n-1 - число РАБОЧИХ процессов
(всего - n+1, n рабочих и 1 управляющий)

Составляются задачи на расчет вида (i, j).
Задача (i, j) означает расчет сил между частицами из блока i и j
(но не в пределах блоков i, j) и расчет скоростей  и перемещений
в блоке i.

1. Процесс запрашивает номер задачи
2. Процесс расчитывает силы - это только часть сил, действующая на 
   точки блока i
3. Процесс суммирует силы через Allgather
4. Процесс находит скорости и перемещения в своем блоке i
5. Процесс рассылает свои скорости и перемещения через Bcast
6. goto 1

*/
void MpiGravSim::CalcStep(float dt)
{
	if (ProcessRank == RootProc)
		rootCalcStep();
	else
		workerCalcStep(dt);
}


Points* MpiGravSim::GetPoints()
{
	return &points;
}




void MpiGravSim::rootCalcStep()
{
	int wtf;
	MPI_Status status;
	currentTask = 0;

	while (currentTask < numTasks)
	{
		MPI_Recv(&wtf, 1, MPI_INT, MPI_ANY_SOURCE, TAG_WORK_REQUEST, MPI_COMM_WORLD, &status);

		MPI_Send(&tasks[currentTask * 2], 2, MPI_INT, status.MPI_SOURCE, TAG_WORK_RESPONSE, MPI_COMM_WORLD);
		currentTask++;
	}

	//std::cout << "#" << ProcessRank << " Done\n";
	//std::cout.flush();
}

void MpiGravSim::workerCalcStep(float dt)
{

	while (true)
	{

		int wtf;
		MPI_Status status;

		//Номера блоков, которые надо расчитать (i, j)
		int task[2];

		//Запрашиваем порцию данных

		MPI_Send(&wtf, 1, MPI_INT, RootProc, TAG_WORK_REQUEST, MPI_COMM_WORLD);
		MPI_Recv(task, 2, MPI_INT, RootProc, TAG_WORK_RESPONSE, MPI_COMM_WORLD, &status);

		//std::cout << "#" << workersProcessRank << "(" << ProcessRank << ") (" << task[0] << ", " << task[1] << ")\n";
		//std::cout.flush();

		//(-1, -1) - нет задачи, завершение
		if (task[0] == -1 && task[1] == -1)
		{
			//std::cout << "#" << workersProcessRank << "(" << ProcessRank << ") Done\n";
			//std::cout.flush();

			break;
		}

		//Считаем силы между частицами из блоков #i, #j
		if (task[0] == task[1])
		{
			/*
			Одинаковые i, j, т.е. находим силы в пределах одного
			блока. Считаем каждую силу один раз.
			*/

			int start = task[0] * pointsPerProcess;

			for (int i = start; i < start + pointsPerProcess; i++)
			{
				for (int j = i+1; j < start + pointsPerProcess; j++)
				{
					float fs[3];
					
					if (!calcForces(i, j, fs))
						continue;
					
					partForces[i * 3 + 0] += fs[0];
					partForces[i * 3 + 1] += fs[1];
					partForces[i * 3 + 2] += fs[2];

					partForces[j * 3 + 0] -= fs[0];
					partForces[j * 3 + 1] -= fs[1];
					partForces[j * 3 + 2] -= fs[2];
				}
			}
		}
		else
		{
			int start1 = task[0] * pointsPerProcess;
			int start2 = task[1] * pointsPerProcess;

			for (int i = start1; i < start1 + pointsPerProcess; i++)
			{
				for (int j = start2; j < start2 + pointsPerProcess; j++)
				{
					float fs[3];

					if (!calcForces(i, j, fs))
						continue;

					partForces[i * 3 + 0] += fs[0];
					partForces[i * 3 + 1] += fs[1];
					partForces[i * 3 + 2] += fs[2];

					partForces[j * 3 + 0] -= fs[0];
					partForces[j * 3 + 1] -= fs[1];
					partForces[j * 3 + 2] -= fs[2];
				}
			}
		}
	}


	//Объединяем силы
	MPI_Allreduce(partForces, points.forces, points.count * 3, MPI_FLOAT, MPI_SUM, workersComm);

	//Расчитываем скорости и перемещения в своем блоке (i)
	for (int i = pointsDisplacement; i < pointsDisplacement + pointsPerProcess; i++)
	{
		//Скорости
		points.speed[i * 3 + 0] += points.forces[i * 3 + 0] / points.mass[i];
		points.speed[i * 3 + 1] += points.forces[i * 3 + 1] / points.mass[i];
		points.speed[i * 3 + 2] += points.forces[i * 3 + 2] / points.mass[i];

		//Перемещения
		points.pos[i * 3 + 0] += points.speed[i * 3 + 0];
		points.pos[i * 3 + 1] += points.speed[i * 3 + 1];
		points.pos[i * 3 + 2] += points.speed[i * 3 + 2];

	}

	//Обнуляем силы
	memset(partForces, 0, sizeof(float) * 3 * points.count);

	//Рассылаем скорости и перемещения
	for (int i = 0; i < workersProcessNum; i++)
	{
		MPI_Bcast(points.speed + i * pointsPerProcess * 3, pointsPerProcess * 3, MPI_FLOAT, i, workersComm);
		MPI_Bcast(points.pos + i * pointsPerProcess * 3, pointsPerProcess * 3, MPI_FLOAT, i, workersComm);
	}
}

//Расчет силы между двумя точками
bool MpiGravSim::calcForces(int i, int j, float* fs)
{

	//Расчет расстояний
	float dx = points.pos[j * 3 + 0] - points.pos[i * 3 + 0];
	float dy = points.pos[j * 3 + 1] - points.pos[i * 3 + 1];
	float dz = points.pos[j * 3 + 2] - points.pos[i * 3 + 2];

	float r_2 = (dx*dx + dy*dy + dz*dz);
	
	if (r_2 < minDist)
		return false;

	r_2 = 1 / r_2;
	float r_1 = sqrt(r_2);

	//Расчет сил
	float f = G*points.mass[i] * points.mass[j] * r_2;

	fs[0] = f*dx*r_1;
	fs[1] = f*dy*r_1;
	fs[2] = f*dz*r_1;

	return true;
}

