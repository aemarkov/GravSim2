#pragma once

#include <SDL2\SDL.h>
#include <GLEW\glew.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Shader.h"
#include "Transofrm.h"

//”прощает работу с OpengGL и SDL
class Engine
{
public:

	Engine(int width, int height, int major_version, int minor_version);
	~Engine();

private:
	
	//system
	SDL_Window* window;
	SDL_GLContext context;
	Shader shader;

	GLuint vbo[2], ibo[1], vao[1];
	const uint32_t positionAttributeIndex = 0, colorAttributeIndex = 1;

	//Uniform
	GLuint gWorldLocation;

	float angle = 0;

	//Methods
	bool init_sdl(int width, int height, int major_version, int minor_version);
	void init_opengl();
	void init_buffers();
	void sdl_loop();

	void render(GLfloat* points, unsigned int points_count, unsigned int * indexes, unsigned int indexes_count);
};