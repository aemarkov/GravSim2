#ifndef __SIMPARAMS_H__
#define __SIMPARAMS_H__

/*
 * \brief Параметры симуляции
 */
struct SimParams
{
	//Число точек
	int ParticlesCount;

	//Число кадров симуляции
	int StepsCount;

	//Шаг симуляции
	float Dt;

	//Выходной файл, куда будет сохранена анимация
	char* OutputFile;

	//Масса одной частицы
	float PointMass;

	//Значение гравитационной постояйнной
	float G;

	//Минимальное расстояние межд частицами
	//Меньше него силы игнорируются
	float MinDist;

	//Радиус зоны спавна частиц
	float Radius;
};

#endif