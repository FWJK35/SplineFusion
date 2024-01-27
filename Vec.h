#pragma once

#include "framework.h"

class Image;
class Coord;

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
	double getSlope();
	Vec interpolate(Vec, double);
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

#include "Coord.h"
#include "Image.h"
