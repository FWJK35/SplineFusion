#pragma once

#include "framework.h"

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

	int getRed();
	int getGreen();
	int getBlue();

	std::string toString();
	void makeGrayScale();
	void threshold(int);
	char* toBytes();
	Pixel operator/(Pixel);
	PrecisePixel operator*(float);
};

#include "PrecisePixel.h"
