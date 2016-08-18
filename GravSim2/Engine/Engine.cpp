#include "Engine.h"

Engine::Engine(int width, int height, int major_version, int minor_version)
	:camera(width/height,45.0,0.1,100)
{
	// Инициализация
	if (!init_sdl(width, height, 3, 2))
		throw "Init failed";

	init_opengl();
	init_buffers();

	// Шейдеры
	shader.Init();
	shader.LoadShader("shaders/vertex.glsl", GL_VERTEX_SHADER);
	shader.LoadShader("shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	shader.LinkProgram();

	//Настройка uniform-перменных
	gWorldLocation = shader.GetUniformLocation("gWorld");

	shader.UseProgram();


	sdl_loop();
}

Engine::~Engine()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool Engine::init_sdl(int width, int height, int major_version, int minor_version)
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

	SDL_GL_SetSwapInterval(1);

	return true;
}

void Engine::init_opengl()
{
	//Use OpenGL 3.0+
	glewExperimental = GL_TRUE;

	glewInit();
}

//init VBO and VBA buffers
void Engine::init_buffers()
{
	//create buffers
	glGenBuffers(2, vbo);
	glGenVertexArrays(1, vao);
	glGenBuffers(1, ibo);
}

//Main drawing and event cycle
void Engine::sdl_loop()
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

	
		/*GLfloat points[] =
		{
			0, 0, 2,
			0, 1, 2,
			1, 0, 2,
			-0.5, 0, 3
		};

		unsigned int indexes[] =
		{
			0, 1, 2,
			0, 1, 3,
			0, 2, 3,
			1, 2, 3
		};*/

		GLfloat points[] =
		{
			-1, -1, 3,
			-1, 1,  3,
			 1, 1,  3,
			 1, -1, 3
		};

		unsigned int indexes[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		render(points, 4, indexes, 6 );

		//refresh window
		glFlush();
		SDL_GL_SwapWindow(window);
	}
}

//Drawing here
void Engine::render(GLfloat* points, unsigned int points_count, unsigned int * indexes, unsigned int indexes_count)
{
	const int pos_index = 0;
	const int color_index = 1;
	GLenum error;

	glBindVertexArray(vao[0]);

	if (indexes != nullptr)
	{
		//Setup indexes
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes_count * sizeof(unsigned int), indexes, GL_STATIC_DRAW);
	}


	//Setup points
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, points_count * 3 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glVertexAttribPointer(pos_index, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pos_index);

	//setup colors
	GLfloat* colors = new GLfloat[points_count * 4];
	for (int i = 0; i < points_count *4; i+=4)
	{
		colors[i] = 0;
		colors[i+1] = 1;
		colors[i+2] = 1;
		colors[i+3] = 1;
	}


	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, points_count * 4 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(color_index, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(color_index);

	//setup uniforms
	camera.SetPosition(0, 0, angle);
	Transform transform = camera.GetTransform();

	angle -= 0.01;

	glUniformMatrix4fv(gWorldLocation, 1, GL_FALSE, glm::value_ptr(transform.GetMat()));

	glClearColor(1, 1, 1, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);

	if(indexes!=nullptr)
		glDrawElements(GL_TRIANGLES, indexes_count, GL_UNSIGNED_INT, 0); 
	else
		glDrawArrays(GL_TRIANGLES, 0, points_count);
	

	delete[] colors;
}
