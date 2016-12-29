#ifndef __GRAVSIM_H__
#define __GRAVSIM_H__

#include "Points.h"
#include <omp.h>
#include <cstdlib>
#include <glm/glm.hpp>

/*!
 * \brief Симулятор гравитации - решение задачи n-тел
 */
class GravSim
{
public:

	GravSim();
	GravSim(int count, glm::vec3 center, glm::vec3 radius);
	~GravSim();

	void Init(int count, glm::vec3 center, glm::vec3 radius);

	void CalcFrameSingleThread(float dt);
	Points* GetPoints();

private:

	Points points;

	//Создает вектор заданного размера
	void init_vector(VectorArray * array, int count);

	//Создает массив Float  заданного размера
	void init_array(float** arr, int count, float default=0);

	//Задает рандомное значение для элементов вектора в заданном радиусе от (0,0,0)
	void set_random_values(VectorArray* values, glm::vec3 radius, glm::vec3 center, int count);

};

#endif