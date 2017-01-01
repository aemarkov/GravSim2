#ifndef __PARTICLE_H__
#define __PARTICLE_H__

struct Particle
{
	float Pos[3];	//Координаты
	float Speed[3];	//Скорости
	float Force[3];	//Ускорения
	float mass;		//Массы
};

#endif