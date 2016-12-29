#include "GravSim.h"

GravSim::GravSim()
{
}

GravSim::GravSim(int count, float pointMass, float G, float minDist, glm::vec3 center, glm::vec3 radius)
{
	Init(count, pointMass, G, minDist, center, radius);
}

GravSim::~GravSim()
{
}

void GravSim::Init(int count, float pointMass, float G, float minDist, glm::vec3 center, glm::vec3 radius)
{
	points.Count = count;
	init_array(&points.Mass, count, pointMass);
	init_vector(&points.Pos, count);
	init_vector(&points.Speed, count);
	init_vector(&forces, count);

	this->G = G;
	this->minDist = minDist;

	set_random_values(&points.Pos, radius, center, count);
}

void GravSim::CalcFrameSingleThread(float dt)
{
	//Расчет сил


	float f;

	for (int i = 0; i < points.Count; i++)
	{
		//Расчет модуля и направления равнодействуюшей
		for (int j = 0; j < points.Count; j++)
		{
			if (i != j)
			{
				//Расчет расстояний
				float dx = points.Pos.X[j] - points.Pos.X[i];
				float dy = points.Pos.Y[j] - points.Pos.Y[i];
				float dz = points.Pos.Z[j] - points.Pos.Z[i];

				float r_2 = (dx*dx + dy*dy + dz*dz);
				if (r_2 < minDist)
					continue;

				r_2 = 1 / r_2;
				float r_1 = sqrt(r_2);

				//Расчет сил
				f = G*points.Mass[i] * points.Mass[j] * r_2;

				forces.X[i] += f*dx*r_1;
				forces.Y[i] += f*dy*r_1;
				forces.Z[i] += f*dz*r_1;
			}
		}
	}

	//Расчет перемещений
	for (int i = 0; i < points.Count; i++)
	{
		points.Speed.X[i] += forces.X[i] / points.Mass[i];
		points.Speed.Y[i] += forces.Y[i] / points.Mass[i];
		points.Speed.Z[i] += forces.Z[i] / points.Mass[i];

		points.Pos.X[i] += points.Speed.X[i];
		points.Pos.Y[i] += points.Speed.Y[i];
		points.Pos.Z[i] += points.Speed.Z[i];

		forces.X[i] = 0;
		forces.Y[i] = 0;
		forces.Z[i] = 0;
	}
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
