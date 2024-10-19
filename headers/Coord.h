#pragma once

#include "framework.h"
class Vec;

class Coord
{
private:
	int x;
	int y;
public:
	Coord();
	Coord(int, int);

	int getX();
	int getY();

	void abs();

	std::string toString();
	double magnitude();
	double magnitudeSquared();
	Vec toVec();

	Coord operator+(Coord c);
	Coord operator-(Coord c);
	Coord operator*(int i);
	void operator+=(Coord c);
	void operator-=(Coord c);
	bool operator==(Coord c);
};

#include "Vec.h"