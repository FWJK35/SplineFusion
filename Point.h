#pragma once

#include "framework.h"
#include "Image.h"
#include "Coord.h"

class Point
{
private:
	double x;
	double y;
public:
	Point();
	Point(double, double);

	double getX();
	double getY();
	double getArea();
	Point round();
	Coord toCoord();
	std::string toString();

	void DrawTo(Image&);

	Point operator+(Point);
	Point operator-(Point);
	Point operator*(double);
	Point operator/(double);

	void operator+=(Point);
	void operator-=(Point);
};
