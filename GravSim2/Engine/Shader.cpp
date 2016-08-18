#include "Shader.h"

Shader::Shader()
{
	
}

void Shader::Init()
{
	shader_program = glCreateProgram();
}

//Загружает и компилирует шейдер
bool Shader::LoadShader(std::string filename, GLenum shader_type)
{
	std::cout << "Loading shader " << filename << std::endl;
	
	//Load code
	GLuint shader = glCreateShader(shader_type);
	std::string shader_code = load_file(filename);
	const char* str = shader_code.c_str();
	int len = shader_code.length();

	glShaderSource(shader, 1, &str, &len);

	//Compile shader
	glCompileShader(shader);

	int was_compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &was_compiled);
	PrintShaderCompilationErrorInfo(shader);

	if (!was_compiled)
		return false;

	shaders.push_back(shader);
	return true;
}

//Использует программу
bool Shader::UseProgram()
{
	//Attach all shaders
	for (GLuint shader : shaders)
		glAttachShader(shader_program, shader);

	glLinkProgram(shader_program);

	int was_linked;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &was_linked);
	if (!was_linked)
	{
		PrintShaderLinkingError(shader_program);
		return false;
	}

	glUseProgram(shader_program);
	return true;
}

//Читает файл в строку
std::string Shader::load_file(std::string filename)
{
	std::ifstream t(filename);
	std::stringstream buffer;
	buffer << t.rdbuf();
	return buffer.str();
}


void Shader::PrintShaderCompilationErrorInfo(int32_t shaderId)
{
	// Find length of shader info log
	int maxLength;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

	// Get shader info log
	char* shaderInfoLog = new char[maxLength];
	glGetShaderInfoLog(shaderId, maxLength, &maxLength, shaderInfoLog);

	std::string log = shaderInfoLog;

	if (log.length())
	{
		std::cout << "=======================================\n";
		std::cout << shaderInfoLog << std::endl;
		std::cout << "=======================================\n\n";
	}

	// Print shader info log
	delete shaderInfoLog;
}

void Shader::PrintShaderLinkingError(int32_t shaderId)
{
	std::cout << "=======================================\n";
	std::cout << "Shader linking failed : " << std::endl;

	// Find length of shader info log
	int maxLength;
	glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

	std::cout << "Info Length : " << maxLength << std::endl;

	// Get shader info log
	char* shaderProgramInfoLog = new char[maxLength];
	glGetProgramInfoLog(shader_program, maxLength, &maxLength, shaderProgramInfoLog);

	std::cout << "Linker error message : " << shaderProgramInfoLog << std::endl;

	// Handle the error by printing it to the console
	delete shaderProgramInfoLog;
	return;
}