#include "PrecisePixel.h"

PrecisePixel::PrecisePixel(float r, float g, float b)
{
	red = r;
	green = g;
	blue = b;
}

Pixel PrecisePixel::round()
{
	red = std::abs(red);
	int newRed = (int)red % 512;
	if (newRed > 255) {
		newRed = 511 - newRed;
	}
	green = std::abs(green);
	int newGreen = (int)green % 512;
	if (newGreen > 255) {
		newGreen = 511 - newGreen;
	}
	blue = std::abs(blue);
	int newBlue = (int)blue % 512;
	if (newBlue > 255) {
		newBlue = 511 - newBlue;
	}
	return Pixel((uint8_t)newRed, (uint8_t)newGreen, (uint8_t)newBlue);
}

std::string PrecisePixel::toString()
{
	std::ostringstream output;
	output << "(" << red << " " << green << " " << blue << ")";
	return output.str();
}

PrecisePixel PrecisePixel::operator*(const float factor)
{
	float newRed = red * factor;
	float newGreen = green * factor;
	float newBlue = blue * factor;

	return PrecisePixel(newRed, newGreen, newBlue);
}

void PrecisePixel::operator+=(PrecisePixel added)
{
	red += added.red;
	green += added.green;
	blue += added.blue;
}
