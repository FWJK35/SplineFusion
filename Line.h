#pragma once

#include "Image.h"
#include "Vec.h"

class Line
{
private:
	Vec start;
	Vec end;
	double thickness;
	double length;
public:
	Line(int, int, int, int);
	Line(Vec, Vec);

	double getLength();

	void DrawTo(Image&);
};

