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
	delete[] points;
}

void GravSim::Init(int count, float pointMass, float G, float minDist, glm::vec3 center, glm::vec3 radius)
{
	pointsCount = count;
	points = new Particle[count];
	
	for (int i = 0; i < count; i++)
	{
		Particle* point = &points[i];
		
		for (int j = 0; j < 3; j++)
			point->Force[j] = 0;

		for (int j = 0; j < 3; j++)
			point->Speed[j] = 0;

		point->mass = pointMass;


		point->Pos[0] = (float)rand() / RAND_MAX * 2 * radius.x - radius.x + center.x;
		point->Pos[1] = (float)rand() / RAND_MAX * 2 * radius.y - radius.y + center.y;
		point->Pos[2] = (float)rand() / RAND_MAX * 2 * radius.z - radius.y + center.z;
	}

	this->G = G;
	this->minDist = minDist;
}

void GravSim::CalcFrameSingleThread(float dt)
{
	//Расчет сил
	float f;

	for (int i = 0; i < pointsCount; i++)
	{
		//Расчет модуля и направления равнодействуюшей
		for (int j = i + 1; j < pointsCount; j++)
		{
			//Расчет расстояний
			float dx = points[j].Pos[0] - points[i].Pos[0];
			float dy = points[j].Pos[1] - points[i].Pos[1];
			float dz = points[j].Pos[2] - points[i].Pos[2];

			float r_2 = (dx*dx + dy*dy + dz*dz);
			if (r_2 < minDist)
				continue;

			r_2 = 1 / r_2;
			float r_1 = sqrt(r_2);

			//Расчет сил
			f = G*points[i].mass * points[j].mass * r_2;

			float fx = f*dx*r_1;
			float fy = f*dy*r_1;
			float fz = f*dz*r_1;

			points[i].Force[0] += fx;
			points[i].Force[1] += fy;
			points[i].Force[2] += fz;

			points[j].Force[0] += fx;
			points[j].Force[1] += fy;
			points[j].Force[2] += fz;
		}
	}

	//Расчет перемещений
	for (int i = 0; i < pointsCount; i++)
	{
		points[i].Speed[0] += points[i].Force[0] / points[i].mass;
		points[i].Speed[1] += points[i].Force[1] / points[i].mass;
		points[i].Speed[2] += points[i].Force[2] / points[i].mass;


		points[i].Pos[0] += points[i].Speed[0];
		points[i].Pos[1] += points[i].Speed[1];
		points[i].Pos[2] += points[i].Speed[2];

		//Обнуляем силы
		points[i].Force[0] = 0;
		points[i].Force[1] = 0;
		points[i].Force[2] = 0;
	}
}


void GravSim::CalcFrameOpenMP(float dt)
{
	//Расчет сил
	float f;

	#pragma omp parallel for
	for (int i = 0; i < pointsCount; i++)
	{
		//Расчет модуля и направления равнодействуюшей
		for (int j = 0; j < pointsCount; j++)
		{
			if (i != j)
			{

				//Расчет расстояний
				float dx = points[j].Pos[0] - points[i].Pos[0];
				float dy = points[j].Pos[1] - points[i].Pos[1];
				float dz = points[j].Pos[2] - points[i].Pos[2];

				float r_2 = (dx*dx + dy*dy + dz*dz);
				if (r_2 < minDist)
					continue;

				r_2 = 1 / r_2;
				float r_1 = sqrt(r_2);

				//Расчет сил
				f = G*points[i].mass * points[j].mass * r_2;

				float fx = f*dx*r_1;
				float fy = f*dy*r_1;
				float fz = f*dz*r_1;

				points[i].Force[0] += fx;
				points[i].Force[1] += fy;
				points[i].Force[2] += fz;
			}
		}
	}

	//Расчет перемещений
	#pragma omp parallel for
	for (int i = 0; i < pointsCount; i++)
	{
		points[i].Speed[0] += points[i].Force[0] / points[i].mass;
		points[i].Speed[1] += points[i].Force[1] / points[i].mass;
		points[i].Speed[2] += points[i].Force[2] / points[i].mass;


		points[i].Pos[0] += points[i].Speed[0];
		points[i].Pos[1] += points[i].Speed[1];
		points[i].Pos[2] += points[i].Speed[2];

		//Обнуляем силы
		points[i].Force[0] = 0;
		points[i].Force[1] = 0;
		points[i].Force[2] = 0;
	}
}



Particle* GravSim::GetPoints()
{
	return points;
}

int GravSim::GetPointsCount()
{
	return pointsCount;
}
