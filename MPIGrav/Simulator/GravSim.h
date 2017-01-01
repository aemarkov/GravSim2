#ifndef __GRAVSIM_H__
#define __GRAVSIM_H__

#include "Particle.h"
#include <omp.h>
#include <cstdlib>
#include <glm/glm.hpp>
#include <iostream>>

/*!
 * \brief ��������� ���������� - ������� ������ n-���
 */
class GravSim
{
public:

	GravSim();
	GravSim(int count, float pointMass, float G, float minDist, glm::vec3 center, glm::vec3 radius);
	~GravSim();

	void Init(int count, float pointMass, float G, float minDist, glm::vec3 center, glm::vec3 radius);

	void CalcFrameSingleThread(float dt);
	void CalcFrameOpenMP(float dt);

	Particle* GetPoints();
	int GetPointsCount();

private:

	Particle* points;
	int pointsCount;

	float G;
	float minDist;
};

#endif