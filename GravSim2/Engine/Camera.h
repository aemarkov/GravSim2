#pragma once

/* Этот класс представляют Камеру.
   Хранит параметры проекции
    - aspect ration
    - обзор
	- плоскости отсечения

   А так же параметры камеры 
    - положение
	- направление
 */

#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Transofrm.h"
#include <iostream>
#include <iomanip>

class Camera
{
public:
	Camera(float aspect_ratio, float fov, float near_clipping, float far_clipping);

	//Задает позицию камеры
	void SetPosition(glm::vec3 pos);
	void SetPosition(float x, float y, float z);

	//Задает направление камеры

	//Камера смотрит на определенную точку
	void SetRotationTarget(glm::vec3 target, glm::vec3 up);

	//Камера смотрит в определенную сторону, заданную вектором
	void SetRotationVector(glm::vec3 direction, glm::vec3 up);

	//Камера смотрит в определенную сторону, заданную углами
	void SetRotationAngles(float yaw, float pitch, float roll, glm::vec3 up);

	//Возвращает объект трансформаций
	Transform GetTransform();

private:
	glm::mat4 camera_matrix, projection_matrix;
	glm::vec3 position, direction, target, up;

	void calc_camera();
};