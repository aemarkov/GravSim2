#include "Engine.h"

Engine::Engine(int width, int height, int major_version, int minor_version, std::function<void(DataToDraw &, float)> callback)
	:camera(width / height, 45.0, 0.1, 100),
	dataToDraw(),
	update_callback(callback)
{

	this->width = width;
	this->height = height;

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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



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
	SDL_WarpMouseInWindow(window, width / 2, height / 2);
	SDL_ShowCursor(0);

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
	const float speed = 0.1;

	bool is_running = true;
	while (is_running) {

		//Check events
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				is_running = false;
				break;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					is_running = false;
					break;

					//Camera control
				case SDLK_w:
					camera.Move(0, 0, speed);
					break;
				case SDLK_s:
					camera.Move(0, 0, -speed);
					break;
				case SDLK_a:
					camera.Move(-speed, 0, 0);
					break;
				case SDLK_d:
					camera.Move(speed, 0, 0);
					break;
				case SDLK_LSHIFT:
					camera.Move(0.0, -speed, 0);
					break;
				case SDLK_SPACE:
					camera.Move(0.0, +speed, 0);
					break;
				}
			}
			else if(event.type == SDL_MOUSEMOTION)
			{

				int x, y;
				SDL_GetMouseState(&x, &y);
				int dx = width / 2 - x;;
				int dy = height / 2 - y;

				SDL_WarpMouseInWindow(window, width / 2, height / 2);
				camera.Rotate(-dx / 10, dy / 10, 0);

				//std::cout << dx << " " << dy << std::endl;
			}
				 
		}


		//RENDER
		if (update_callback != nullptr)
			update_callback(dataToDraw, 0);

		render(dataToDraw);

		//refresh window
		glFlush();
		SDL_GL_SwapWindow(window);
	}
}


void Engine::render(GLfloat* points, unsigned int points_count, unsigned int * indexes, unsigned int indexes_count)
{
	render(DataToDraw(points, points_count, indexes, indexes_count));
}


//Drawing here
void Engine::render(DataToDraw dataToDraw)
{
	glClearColor(1, 1, 1, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);

	if (dataToDraw.PointsCount == 0)
		return;


	const int pos_index = 0;
	const int color_index = 1;
	GLenum error;

	glBindVertexArray(vao[0]);

	if (dataToDraw.Indexes != nullptr)
	{
		//Setup indexes
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataToDraw.IndexesCount * sizeof(unsigned int), dataToDraw.Indexes, GL_STATIC_DRAW);
	}


	//Setup points
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, dataToDraw.PointsCount * 3 * sizeof(GLfloat), dataToDraw.Points, GL_STATIC_DRAW);
	glVertexAttribPointer(pos_index, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pos_index);

	//setup colors
	GLfloat* colors = new GLfloat[dataToDraw.PointsCount * 4];
	for (int i = 0; i < dataToDraw.PointsCount *4; i+=4)
	{
		colors[i] = 1;
		colors[i+1] = 0;
		colors[i+2] = 0;
		colors[i+3] = 1;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, dataToDraw.PointsCount * 4 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(color_index, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(color_index);

	//setup uniforms
	Transform transform = camera.GetTransform();

	glUniformMatrix4fv(gWorldLocation, 1, GL_FALSE, glm::value_ptr(transform.GetMat()));

	if(dataToDraw.Indexes!=nullptr)
		glDrawElements(GL_LINES, dataToDraw.IndexesCount, GL_UNSIGNED_INT, 0); 
	else
		glDrawArrays(GL_TRIANGLES, 0, dataToDraw.PointsCount);
	

	delete[] colors;
}
