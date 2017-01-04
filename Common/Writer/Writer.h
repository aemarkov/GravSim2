#ifndef __WRITER_H__
#define __WRITER_H__

/*
 * \brief Записывает данные симуляции
*/

#include <fstream>
#include <ios>
#include "Frame.h"

class Writer
{
public:
	Writer();
	Writer(unsigned int pointsCount, const char* filename);
	~Writer();

	/*!
	 * \brief Открывает файл и записывает в него количество точек 
	 */
	void Open(unsigned int pointsCount, const char* filename);

	/*!
	 * \brief Записывает кадр - массив точек и время относительно 
	 * предыдущего кадра
	 *
	 * ВНИМАНИЕ: УБЕДИТЕСЬ, ЧТО БУФЕР POINTS В FRAME РАЗМЕРОМ
	 * pointsCount
	 */
	void WriteFrame(DataTypes::Frame & frame);

	void Close();

private:
	unsigned int pointsCount;
	std::ofstream stream;
};

#endif