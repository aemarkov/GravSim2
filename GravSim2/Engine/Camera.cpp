#include "Camera.h"

Camera::Camera(float aspect_ratio, float fov, float near_clipping, float far_clipping):position(0,0,0),target(0,0,0),direction(0,0,1),up(0,1,0)
{
	//Создание матрицы проекции
	float z_range = far_clipping - near_clipping;
	float tag_half_fov = tanf(glm::radians(fov / 2));

	projection_matrix = glm::mat4(0);
	projection_matrix[0][0] = 1 / (aspect_ratio*tag_half_fov);;
	projection_matrix[1][1] = 1 / tag_half_fov;
	projection_matrix[2][2] = (near_clipping + far_clipping) / z_range;;
	projection_matrix[2][3] = 1;
	projection_matrix[3][2] = (-2 * far_clipping*near_clipping) / z_range;;;


	calc_camera();
}

//Задает положение
void Camera::SetPosition(glm::vec3 pos)
{
	this->position = pos;
	calc_camera();
}

void Camera::SetPosition(float x, float y, float z)
{
	this->position = glm::vec3(x, y, z);
	calc_camera();
}


//Задает направление взгляда вектором
void  Camera::SetRotationVector(glm::vec3 direction, glm::vec3 up)
{
	this->direction = direction;
	this->up = up;

	calc_camera();
}

//Задает направление взгляда углами
void Camera::SetRotationAngles(float yaw, float pitch, float roll, glm::vec3 up)
{
	glm::vec3 direction;

	direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	direction.y = sin(glm::radians(pitch));
	direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	SetRotationVector(direction, up);
}

//Создает новый объект трансформации, который будет применять матрицы проекции и камеры
Transform Camera::GetTransform()
{
	return Transform(camera_matrix, projection_matrix);
}


void print(glm::mat4 mat)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout << std::setw(6) << std::setprecision(2) << mat[i][j] << " ";

		std::cout << std::endl;
	}
}

//Расчитывает матрицу камеры
void Camera::calc_camera()
{	
	//Вот это все теперь полностью аналогично lookAt
	/*direction = glm::normalize(direction);
	up = glm::normalize(up);

	glm::vec3 right = glm::normalize(glm::cross(up,direction));
	glm::vec3 up = glm::normalize(glm::cross(direction,right));
	target = direction;

	camera_matrix[0][0] = right.x; camera_matrix[0][1] = up.x; camera_matrix[0][2] = target.x; camera_matrix[0][3] = -position.x;
	camera_matrix[1][0] = right.y; camera_matrix[1][1] = up.y; camera_matrix[1][2] = target.y; camera_matrix[1][3] = -position.y;
	camera_matrix[2][0] = right.z; camera_matrix[2][1] = up.z; camera_matrix[2][2] = target.z; camera_matrix[2][3] = -position.z;
	camera_matrix[3][0] = 0;       camera_matrix[3][1] = 0;    camera_matrix[3][2] = 0;        camera_matrix[3][3] = 1;

	camera_matrix = glm::transpose(camera_matrix);*/

	
	camera_matrix = glm::lookAt(position, position-direction, up);
}
