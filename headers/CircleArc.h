#pragma once

#include "framework.h"

#include "Vec.h"
#include "Image.h"

class CircleArc
{
private:
	Vec start;
	Vec end;
	double curvature;

	Vec center;
	double radius;
	double length;
public:
	CircleArc(Vec, Vec, double);

	void DrawTo(Image&);

	static Vec GetCenter(Vec, Vec, Vec);
};

