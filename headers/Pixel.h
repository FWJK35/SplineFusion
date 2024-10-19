#pragma once

#include "framework.h"
#include "Coord.h"

class PrecisePixel;

class Pixel
{
private:
	uint8_t red;
	uint8_t green;
	uint8_t blue;
public:
	Pixel();
	Pixel(uint8_t, uint8_t, uint8_t);
	Pixel(uint8_t);

	int getRed();
	int getGreen();
	int getBlue();
	double getAvg();

	std::string toString();
	void makeGrayScale();
	void increaseConrast(double);
	void threshold(int);
	char* toBytes();
	void operator+=(Pixel);
	Pixel operator/(Pixel);
	PrecisePixel operator*(float);
};

#include "PrecisePixel.h"
