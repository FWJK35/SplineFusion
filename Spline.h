#pragma once

#include "framework.h"

#include "Vec.h"
#include "Image.h";

class Spline
{
private:
	Vec start;
	Vec controlA;
	Vec controlB;
	Vec end;

	double length;

public:
	Spline(Vec, Vec, Vec, Vec);

	Vec getPointAlong(double);

	void drawTo(Image&);
};

