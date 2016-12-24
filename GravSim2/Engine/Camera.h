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
	
	//Относительное смещение
	void Move(glm::vec3 dpos);
	void Move(float dx, float dy, float dz);

	//Задает направление камеры
	//Камера смотрит в определенную сторону, заданную вектором
	void SetRotationVector(glm::vec3 direction, glm::vec3 up);

	//Камера смотрит в определенную сторону, заданную углами
	void SetRotationAngles(glm::vec3 angles);
	void SetRotationAngles(float yaw, float pitch, float roll);

	//Относительное вращение
	void Rotate(glm::vec3 d_rot);
	void Rotate(float d_yaw, float d_pitch, float d_roll);


	//Возвращает объект трансформаций
	Transform GetTransform();

private:
	glm::mat4 camera_matrix, projection_matrix;
	glm::vec3 position, direction, up;
	glm::vec3 angles;

	void calc_camera();
};