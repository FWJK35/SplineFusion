#pragma once

#include "framework.h"

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

	Coord operator+(Coord c);
	Coord operator-(Coord c);
};