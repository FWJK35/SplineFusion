#pragma once

#include "framework.h"
#include "Image.h"
#include "Coord.h"

class Vec
{
private:
	double x;
	double y;
public:
	Vec();
	Vec(double, double);

	double getX();
	double getY();
	void setX(double);
	void setY(double);
	Vec round();
	Coord toCoord();
	std::string toString();

	double getArea();
	Vec getMidPoint(Vec);
	double getDistance(Vec);
	Vec invert();

	void DrawTo(Image&);

	Vec operator+(Vec);
	Vec operator-(Vec);
	Vec operator*(double);
	Vec operator/(double);

	void operator+=(Vec);
	void operator-=(Vec);
};
