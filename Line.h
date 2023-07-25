#pragma once

#include "Image.h"
#include "Point.h"

class Line
{
private:
	Point start;
	Point end;
	double thickness;
	double length;
public:
	Line(int, int, int, int);
	Line(Point, Point);

	double getLength();

	void DrawTo(Image&);
};

