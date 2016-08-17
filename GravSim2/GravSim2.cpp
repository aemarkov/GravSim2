// GravSim2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <SDL2\SDL.h>
#include <GLEW\glew.h>


SDL_Window* window;
SDL_GLContext context;

bool init_sdl(int width, int height, int major_version, int minor_version);
void init_opengl();
void sdl_loop();
void CleanUp();


int main(int argc, char *argv[])
{
	int width = 640;
	int height = 480;

	auto window = init_sdl(width, height, 3,2);
	if (!window)
		return 0;

	init_opengl();
	
	sdl_loop();

	CleanUp();

    return 0;
}

//init the SDL
bool init_sdl(int width, int height, int major_version, int minor_version)
{
	//Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Create window
	window = SDL_CreateWindow("GravSim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if (window == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Disable all depreated shit
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Set OpenGL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major_version);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor_version);

	//Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	context = SDL_GL_CreateContext(window);

	return true;
}

void init_opengl()
{
	//Use OpenGL 3.0+
	glewExperimental = GL_TRUE;
	
	glewInit();
}

//Main loop
void sdl_loop()
{
	bool is_running = true;
	while (is_running) {

		//Check events
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				is_running = false;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					is_running = false;
					break;
				}
				break;
			}
		}

		//Draw
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//refresh window
		glFlush();
		SDL_GL_SwapWindow(window);
	}
}

//Delete all stuff
void CleanUp()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}