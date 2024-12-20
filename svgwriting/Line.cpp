#include "Line.h"

/*
* Creates a new line with given x and y position endpoints
*/
Line::Line(int x1, int y1, int x2, int y2) : Line(Vec(x1, y1), Vec(x2, y2))
{

}

/*
* Creates a line with given Vector position endpoints
*/
Line::Line(Vec aStart, Vec aEnd)
{
	start = aStart;
	end = aEnd;
	thickness = 1;
	Vec dist = (end - start);
	length = std::sqrt(dist.getX() * dist.getX() + dist.getY() * dist.getY());
}

/*
* Gets the length of this line
*/
double Line::getLength()
{
	return length;
}

/*
* Draws this line to a given image
*/
void Line::DrawTo(Image& img)
{
	Vec currentPoint = start;
	Vec step = (end - start) / (int) length;
	for (int d = 0; d < length; d++) {

		//std::cout << d << std::endl;
		currentPoint.DrawTo(img);
		currentPoint += step;
	}
}
