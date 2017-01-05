#ifndef __GRAVSIM_H__
#define __GRAVSIM_H__

#include "../mpicommon.h"
#include "Points.h"
#include "Point.h"

#include <glm/glm.hpp>
#include <mpi.h>
#include <iostream>
#include <omp.h>

class MpiGravSim
{
public:

	MpiGravSim();
	MpiGravSim(int count, float pointMass, float G, float minDist, glm::vec3 center, glm::vec3 radius);
	~MpiGravSim();

	void Init(int count, float pointMass, float G, float minDist, glm::vec3 center, glm::vec3 radius);

	//Расчет шага симуляции
	void CalcStep(float dt);

	Points* GetPoints();

private:

	//MPI
	MPI_Comm workersComm;
	int workersProcessNum;		//число рабочих процессов
	int workersProcessRank;		//Ранк рабочего процесса в коммуникаторе

	int numThreads;				//Число потоков OpenMP

	int pointsPerProcess;		//Число точек в блоке (на процесс)
	int pointsDisplacement;		//Смещение каждого блока

	int* tasks;					//Задачи
	int numTasks;				//Число задач
	int currentTask;			//Текущая задача

	Points points;				//Данные точек
	float* tempForces;			//Временные силы для потока
	float* partForces;			//Временные силы для процесса

	float G;
	float minDist;

	//Теги сообщение
	const int TAG_WORK_REQUEST = 1;				//Запрос очередной задачи
	const int TAG_WORK_RESPONSE = 2;			//Передача задачи




	//Функция главного процесса - раздача задач
	void rootCalcStep();

	//Функция рабочих процессов - расчет
	void workerCalcStep(float dt);

	/*! 
	 * \brief Расчет силы между двумя точками
	 *
	 * param[in] pi - индекс точки в общем массиве (со смещением)
	 * param[in] pj - индекс точки в общем массиве (со смещением)
	 * param[in] tfi - индекс в массиве временных сил (без смещения)
	 * param[in] tfj - индекс в массиве временных сил (без смещения)
	 * param[in] thread - номер потока
	 */
	bool calcForces(int pi, int pj,  int tfj, int thread);
};

#endif