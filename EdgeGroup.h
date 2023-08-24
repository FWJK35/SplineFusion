#pragma once

#include "framework.h"
#include "Coord.h"
#include "ImageData.h"

class EdgeGroup
{
private:
	std::vector<Coord> points;
	double avgDirection;
	double variation;

public:
	static double compareAngles(double, double);

	EdgeGroup(int, int, ImageData&, ImageData&);

	void printData();
	int getSize();
	double getAvgDirection();
	double getVariation();
	std::vector<Coord> getPoints();
	
	void StemFrom(int, int, ImageData&, ImageData&);
};

