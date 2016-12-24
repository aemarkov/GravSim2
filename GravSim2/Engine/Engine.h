#pragma once

#include <SDL2\SDL.h>
#include <GLEW\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>
#include <functional>


#include "Shader.h"
#include "Transofrm.h"
#include "Camera.h"
#include "DataToDraw.h"

//Упрощает работу с OpengGL и SDL
class Engine
{
public:

	Engine(int width, int height, int major_version, int minor_version, std::function<void(DataToDraw &, float)> callback);
	~Engine();

private:
	
	//system
	SDL_Window* window;
	SDL_GLContext context;
	int width, height;

	//Объект, отвечающий за шейдеры
	Shader shader;						

	//Камера
	Camera camera;

	//Буферы для создания вершин
	GLuint vbo[2], ibo[1], vao[1];

	//Uniform
	GLuint gWorldLocation;

	//Data to draw
	DataToDraw dataToDraw;

	//Callback for update data
	std::function<void(DataToDraw &, float)> update_callback;
	
	//Methods
	bool init_sdl(int width, int height, int major_version, int minor_version);
	void init_opengl();
	void init_buffers();
	void sdl_loop();

	void render(GLfloat* points, unsigned int points_count, unsigned int * indexes, unsigned int indexes_count);
	void render(DataToDraw data);
};
