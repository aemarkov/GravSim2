#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GLEW\glew.h>

class Shader
{
public:

	Shader();

	void Init();

	//Загружает и компилирует шейдер
	bool LoadShader(std::string filename, GLenum shader_type);
	
	//Использует программу
	bool UseProgram();
private:
	
	//Читает файл в строку
	std::string load_file(std::string filename);

	//Печатает ошибки компиляции
	void PrintShaderCompilationErrorInfo(int32_t shaderId);

	//Печатает ошибки линковки
	void PrintShaderLinkingError(int32_t shaderId);

	std::vector<GLuint> shaders;
	GLuint shader_program;
};