#include "Transofrm.h"

Transform::Transform() : scale(1,1,1)
{
}

void Transform::Scale(float x, float y, float z)
{
	scale = glm::vec3(x, y, z);

	/*scale_mat = glm::mat4(1);
	scale_mat[0][0] = x;
	scale_mat[1][1] = y;
	scale_mat[2][2] = z;*/
}

void Transform::Move(float x, float y, float z)
{
	move = glm::vec3(x, y, z);

	/*move_mat = glm::mat4(1);
	move_mat[3][0] = x;
	move_mat[3][1] = y;
	move_mat[3][2] = z;*/
}

void Transform::Rotate(float x, float y, float z)
{
	rot = glm::vec3(x, y, z);

	/*rot_mat = glm::mat4(1);
	rot_mat[0][0] = x;
	rot_mat[1][1] = y;
	rot_mat[2][2] = z;*/
}

glm::mat4x4 Transform::GetMat()
{
	glm::mat4 mat(1);
	mat = glm::scale(mat, scale);
	mat = glm::rotate(mat, rot.x, glm::vec3(1, 0, 0));
	mat = glm::rotate(mat, rot.y, glm::vec3(0, 1, 0));
	mat = glm::rotate(mat, rot.z, glm::vec3(0, 0, 1));
	mat = glm::translate(mat, move);

	return mat;
}