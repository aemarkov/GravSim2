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



//Buffers for ordynary drawing
class DataBuffers
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

	void Start();
	void SetupStaticData(DataToDraw data);

private:
	
	//system
	SDL_Window* window;
	SDL_GLContext context;
	int width, height;

	//Объект, отвечающий за шейдеры
	Shader shader;						

	//Камера
	Camera camera;

	DataBuffers staticBuffers;
	DataBuffers pointsBuffers;
		
	//Uniform
	GLuint gWorldLocation;

	//Data to draw
	DataToDraw staticData;
	DataToDraw pointsData;

	//Callback for update data
	std::function<void(DataToDraw &, float)> update_callback;
	
	//Methods
	bool init_sdl(int width, int height, int major_version, int minor_version);
	void init_opengl();
	void sdl_loop();

	//Buffers setup
	void init_static_buffers(DataBuffers & buffers);
	void init_points_buffer(DataBuffers & buffers, int bufferSize);


	void render(GLfloat* points, unsigned int points_count, unsigned int * indexes, unsigned int indexes_count);
	void render(DataToDraw data);

	void renderStatic(DataToDraw dataToDraw, DataBuffers & buffers);
	void renderPoints(DataToDraw & dataToDraw, DataBuffers & buffers);
};
