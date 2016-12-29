#include <iostream>
#include "Writer\Writer.h"
#include "Frame.h"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Usage:: MPIGrav.exe <output file>";
		return -1;
	}

	int count = 3000;
	Writer writer(count, argv[1]);

	Frame frame;
	frame.Points = new Point[count];
	frame.Count = count;
	//frame.Dt = 1;

	for (int f= 0; f < 1000; f++)
	{
		for (int i = 0; i < count; i++)
		{
			frame.Points[i].X = (rand() % 600) / 100.0 - 3;
			frame.Points[i].Y = (rand() % 600) / 100.0 - 3;
			frame.Points[i].Z = (rand() % 600) / 100.0 - 3;
		}

		frame.Dt = f;
		writer.WriteFrame(frame);
	}

    return 0;
}

