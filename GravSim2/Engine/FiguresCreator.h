#ifndef __FIGURESCREATOR_H__
#define __FIGURESCREATOR_H__


#include <glm\glm.hpp>
#include <GLEW\glew.h>
#include <vector>
#include <math.h>  

#include "DataToDraw.h"

#define M_PI       3.14159265358979323846   // pi

/*!
 * \brief Create simple figures
 */
class FiguresCreator
{
public:

	FiguresCreator();
	~FiguresCreator();

	void DrawCircle(glm::vec3 center, float radius, int count);
	void DrawLine(glm::vec3 start, glm::vec3 end);
	
	void GetData(DataToDraw & data);
	
private:

	std::vector<GLfloat> points;
	std::vector<unsigned int> indexes;

	void addFigure(DataToDraw & figure);
};

#endif 