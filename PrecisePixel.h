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

	float getRed();
	float getGreen();
	float getBlue();
	double getAvg();

	Pixel round();
	std::string toString();
	PrecisePixel operator*(const float);
	void operator+=(PrecisePixel);

	static PrecisePixel fromRGB(double, double, double);
	static PrecisePixel fromRGB(PrecisePixel);
	static PrecisePixel fromHSV(double, double, double);
	static PrecisePixel fromHSV(PrecisePixel);
};

#include "Pixel.h"
