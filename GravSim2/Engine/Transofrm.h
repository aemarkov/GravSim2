#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

//Класс для упрощения 3Д трансформаций
class Transform
{
public:
	Transform();
	void Scale(float x, float y, float z);
	void Move(float x, float y, float z);
	void Rotate(float x, float y, float z);
	
	glm::mat4x4 GetMat();
	float * GetPointer();

private:

	glm::vec3 scale, rot, move;
	//glm::mat4 scale_mat, rot_mat, move_mat;
};
