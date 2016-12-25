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

class ParticleSystem
{
public:
	unsigned int MaxCount;
	unsigned int Count;
	GLfloat* Positions;
	GLfloat* Colors;
};

//Buffers for drawing particle system
class ParticleSystemBuffers
{
public:
	GLuint ParticleVertexBuffer;
	GLuint ParticlePositionsBuffer;
	GLuint ParticleColorsBuffer;
};

//Buffers for ordynary drawing
class OrdinaryBuffers
{
public:
	GLuint VertexBufer;
	GLuint ColorBuffer;
	GLuint IndexBuffer;
	GLuint vao;
};

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

	ParticleSystemBuffers particleSystem;
	OrdinaryBuffers ordinaryBuffers;
		
	//Uniform
	GLuint gWorldLocation;

	//Data to draw
	DataToDraw dataToDraw;

	//Callback for update data
	std::function<void(DataToDraw &, float)> update_callback;
	
	//Methods
	bool init_sdl(int width, int height, int major_version, int minor_version);
	void init_opengl();
	void sdl_loop();

	//Buffers setup
	void init_ordynary_buffers(OrdinaryBuffers & buffers);
	void init_particle_system_buffer(ParticleSystemBuffers & buffers, int bufferSize);


	void render(GLfloat* points, unsigned int points_count, unsigned int * indexes, unsigned int indexes_count);
	void render(DataToDraw data);

	void renderOrdinary(DataToDraw dataToDraw, OrdinaryBuffers & buffers);
	void renderParticleSystem(ParticleSystem & system, ParticleSystemBuffers & buffers);
};
