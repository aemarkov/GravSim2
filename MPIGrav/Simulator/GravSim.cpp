#include "GravSim.h"

GravSim::GravSim()
{
}

GravSim::GravSim(int count, glm::vec3 center, glm::vec3 radius)
{
	Init(count, center, radius);
}

GravSim::~GravSim()
{
}

void GravSim::Init(int count, glm::vec3 center, glm::vec3 radius)
{
	points.Count = count;
	init_array(&points.Mass, count, 1);
	init_vector(&points.Pos, count);
	init_vector(&points.Speed, count);

	set_random_values(&points.Pos, radius, center, count);
}

void GravSim::CalcFrameSingleThread(float dt)
{
}

Points* GravSim::GetPoints()
{
	return &points;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

//Создает вектор заданного размера
void GravSim::init_vector(VectorArray * vector, int count)
{
	init_array(&(vector->X), count);
	init_array(&(vector->Y), count);
	init_array(&(vector->Z), count);
}

//Создает массив Float  заданного размера
void GravSim::init_array(float ** arr, int count, float default)
{
	*arr = new float[count];

	//memset нельзя для float
	for (int i = 0; i < count; i++)
		(*arr)[i] = default;
}

//Задает рандомное значение для элементов вектора в заданном радиусе от (0,0,0)
void GravSim::set_random_values(VectorArray * values, glm::vec3 radius, glm::vec3 center, int count)
{
	for (int i = 0; i < count; i++)
	{
		values->X[i] = (float)rand() / RAND_MAX * 2 * radius.x - radius.x + center.x;
		values->Y[i] = (float)rand() / RAND_MAX * 2 * radius.y - radius.y + center.y;
		values->Z[i] = (float)rand() / RAND_MAX * 2 * radius.z - radius.y + center.z;
	}
}
