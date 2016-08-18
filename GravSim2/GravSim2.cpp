// GravSim2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <SDL2\SDL.h>
#include <GLEW\glew.h>

#include "Engine\Engine.h"

int main(int argc, char *argv[])
{
	Engine engine(640, 480, 3,3);
    return 0;
}
