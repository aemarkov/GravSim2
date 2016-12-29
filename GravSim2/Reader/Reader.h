#ifndef __READER_H__
#define __READER_H__

#include <fstream>
#include "Frame.h"

class Reader
{
public:
	Reader();
	Reader(const char* filename);
	~Reader();

	/*! 
	 * \brief Открывает файл
	 */
	unsigned int Open(const char* filename);

	/*!
	 * \brief Возвращает количество точек в кадре
	 */
	unsigned int GetPointsCount();
	void Close();

	/*!
	 * \brief Считывает один кадр из файла и 
	 *        выделяет каждый раз память под объект
	 *	 	  Frame
	 */
	Frame* ReadFrame();

	/*!
	 * \brief Считывает один кадр из файла
	 *        и записывает его по укзаателю
	 *		  на Frame. 
	 *
	 * ВНИМАНИЕ:
	 * УБЕДИТЕСЬ, ЧТО ВО FRAME ВЫДЕЛЕН БУФЕР
     * POINTS ДОСТАТОЧНОГО РАЗМЕРА
 	 */
	bool ReadFrame(Frame* frame);

private:

	std::ifstream stream;
	unsigned int pointsCount;
};

#endif