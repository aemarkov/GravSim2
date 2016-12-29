#include "Writer.h"

Writer::Writer()
{
}

Writer::Writer(unsigned int pointsCount, char * filename):stream(filename, std::ios::out|std::ios::binary)
{
	if(!stream)
		throw std::string("Unable to open file \"") + std::string(filename) + std::string("\"");

	this->pointsCount = pointsCount;
	stream.write((const char*)&pointsCount, sizeof(pointsCount));

}

Writer::~Writer()
{
	Close();
}

/*!
* \brief Открывает файл и записывает в него количество точек
*/
void Writer::Open(unsigned int pointsCount, char * filename)
{
	stream.open(filename, std::ios::out | std::ios::binary);

	if (!stream)
		throw std::string("Unable to open file \"") + std::string(filename) + std::string("\"");

	this->pointsCount = pointsCount;
	stream.write((const char*)&pointsCount, sizeof(pointsCount));
}

/*!
* \brief Записывает кадр - массив точек и время относительно
* предыдущего кадра
*/
void Writer::WriteFrame(Frame & frame)
{
	stream.write((const char*)&frame.Dt, sizeof(frame.Dt));
	stream.write((const char*)frame.Points, sizeof(Point)*pointsCount);
}

void Writer::Close()
{
	if (stream.is_open())
		stream.close();
}
