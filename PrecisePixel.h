#pragma once

#include "framework.h"
class Pixel;

class PrecisePixel
{
private:
	float red;
	float green;
	float blue;
public:
	PrecisePixel(float, float, float);
	Pixel round();
	std::string toString();
	PrecisePixel operator*(const float);
	void operator+=(PrecisePixel);
};

#include "Pixel.h"
