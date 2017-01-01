#include "Engine.h"

Engine::Engine(int width, int height, int major_version, int minor_version, std::function<void(DataToDraw &, float)> callback)
	:camera(width / height, 45.0, 0.5, 5000),
	staticData(),
	pointsData(),
	update_callback(callback)
{

	this->width = width;
	this->height = height;

	// Инициализация
	if (!init_sdl(width, height, 3, 2))
		throw "Init failed";

	init_opengl();
	init_static_buffers(staticBuffers);
	init_points_buffer(pointsBuffers, 100);

	// Шейдеры
	shader.Init();
	shader.LoadShader("shaders/vertex.glsl", GL_VERTEX_SHADER);
	shader.LoadShader("shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	shader.LinkProgram();

	//Настройка uniform-перменных
	gWorldLocation = shader.GetUniformLocation("gWorld");

	shader.UseProgram();

	camera.SetPosition(0, 100, 0);
	camera.SetRotationAngles(0, -85, 0);
}

Engine::~Engine()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Engine::Start()
{
	sdl_loop();
}

void Engine::SetupStaticData(DataToDraw data)
{
	staticData = data;
}


//////////////////////////////////////////// INITIALIZATION ////////////////////////////////////////////|||||||||||||||



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

	glEnable(GL_PROGRAM_POINT_SIZE);

	return true;
}

void Engine::init_opengl()
{
	//Use OpenGL 3.0+
	glewExperimental = GL_TRUE;

	glewInit();
}

////////////////////////////////////////////// BUFFERS INITIALIZATION /////////////////////////////////////////////////

void Engine::init_static_buffers(DataBuffers & buffers)
{
	glGenBuffers(1, &buffers.VertexBufer);
	glGenBuffers(1, &buffers.ColorBuffer);
	glGenVertexArrays(1, &buffers.vao);
	glGenBuffers(1, &buffers.IndexBuffer);
}

void Engine::init_points_buffer(DataBuffers & buffers, int bufferSize)
{
	static const GLfloat g_vertex_buffer_data[] =
	{
		-0.05f, -0.05f, 0.0f,
		0.05f, -0.05f, 0.0f,
		-0.05f, 0.05f, 0.0f,
		0.05f, 0.05f, 0.0f,
	};


	//Position buffer
	//It will be fill every frame, now - NULL
	glGenBuffers(1, &buffers.VertexBufer);
	glBindBuffer(GL_ARRAY_BUFFER, buffers.VertexBufer);
	glBufferData(GL_ARRAY_BUFFER, bufferSize * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &buffers.ColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffers.ColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, bufferSize * 1 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
}

////////////////////////////////////////////// EVENT SYSTEM ///////////////////////////////////////////////////////////

//Main drawing and event cycle
void Engine::sdl_loop()
{
	const float speed =1;

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
			update_callback(pointsData, 0);

		render(staticData);

		//refresh window
		glFlush();
		SDL_GL_SwapWindow(window);
	}
}


////////////////////////////////////////////////// RENDERING //////////////////////////////////////////////////////////

void Engine::render(GLfloat* points, unsigned int points_count, unsigned int * indexes, unsigned int indexes_count)
{
	render(DataToDraw(points, points_count, indexes, indexes_count));
}


//Drawing here
void Engine::render(DataToDraw dataToDraw)
{
	glClearColor(0, 0, 0, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);

	//setup uniforms
	Transform transform = camera.GetTransform();

	glUniformMatrix4fv(gWorldLocation, 1, GL_FALSE, glm::value_ptr(transform.GetMat()));

	
	renderStatic(dataToDraw, staticBuffers);
	renderPoints(pointsData, pointsBuffers);
}

//Render static lines
void Engine::renderStatic(DataToDraw dataToDraw, DataBuffers & buffers)
{
	if (dataToDraw.PointsCount == 0)
		return;


	const int pos_index = 0;
	const int color_index = 1;
	GLenum error;

	glBindVertexArray(buffers.vao);

	
	//Setup indexes
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataToDraw.IndexesCount * sizeof(unsigned int), dataToDraw.Indexes, GL_STATIC_DRAW);
	


	//Setup points
	glBindBuffer(GL_ARRAY_BUFFER, buffers.VertexBufer);
	glBufferData(GL_ARRAY_BUFFER, dataToDraw.PointsCount * 3 * sizeof(GLfloat), dataToDraw.Points, GL_STATIC_DRAW);
	glVertexAttribPointer(pos_index, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pos_index);

	//setup colors
	GLfloat* colors = new GLfloat[dataToDraw.PointsCount * 4];
	for (int i = 0; i < dataToDraw.PointsCount; i++)
	{
		colors[i] = 255;
	}

	glBindBuffer(GL_ARRAY_BUFFER, buffers.ColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, dataToDraw.PointsCount * 1 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(color_index, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(color_index);


	glDrawElements(GL_LINES, dataToDraw.IndexesCount, GL_UNSIGNED_INT, 0);

	delete[] colors;
}

//Render dynamic points
void Engine::renderPoints(DataToDraw & dataToDraw, DataBuffers & buffers)
{

	if (dataToDraw.PointsCount == 0)
		return;


	const int pos_index = 0;
	const int color_index = 1;
	GLenum error;

	glBindVertexArray(buffers.vao);

	//Setup points
	glBindBuffer(GL_ARRAY_BUFFER, buffers.VertexBufer);
	glBufferData(GL_ARRAY_BUFFER, dataToDraw.PointsCount * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, dataToDraw.PointsCount * 3 * sizeof(GLfloat), dataToDraw.Points);
	glVertexAttribPointer(pos_index, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pos_index);

	//setup colors
	/*GLfloat* colors = new GLfloat[dataToDraw.PointsCount];
	for (int i = 0; i < dataToDraw.PointsCount * 1; i++)
	{
		colors[i] = COLOR(255,255,255);
	}*/

	glBindBuffer(GL_ARRAY_BUFFER, buffers.ColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, dataToDraw.PointsCount * 1 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, dataToDraw.PointsCount * 1 * sizeof(GLfloat), dataToDraw.Colors);
	glVertexAttribPointer(color_index, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(color_index);


	glDrawArrays(GL_POINTS, 0, dataToDraw.PointsCount);

	//delete[] colors;

}