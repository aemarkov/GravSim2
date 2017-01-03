#ifndef __VECTORARRAY_H__
#define __VECTORARRAY_H__

/*!
 * \brief Координаты точек в отдельных массивах
 *        для удобства передачи, векторизации итп.
 */
struct VectorArray
{
	float* X;
	float* Y;
	float* Z;

	~VectorArray();
};

#endif
