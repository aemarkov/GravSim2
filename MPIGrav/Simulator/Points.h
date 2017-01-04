#ifndef __POINTS_H__
#define __POINTS_H__

struct Points
{
	float* mass;
	float* forces;
	float* speed;
	float* pos;

	int count;
};

#endif