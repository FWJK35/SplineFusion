#include "PrecisePixel.h"

PrecisePixel::PrecisePixel(float r, float g, float b)
{
	red = r;
	green = g;
	blue = b;
}

float PrecisePixel::getRed()
{
	return red;
}

float PrecisePixel::getGreen()
{
	return green;
}

float PrecisePixel::getBlue()
{
	return blue;
}

double PrecisePixel::getAvg()
{
	return (red + green + blue) / 3.0;
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

PrecisePixel PrecisePixel::fromHSV(double hue, double sat, double val)
{
	double M = 255 * val;
	double m = M * (1 - sat);
	double z = (M - m) * (1 - std::abs(std::fmod(hue / 60, 2) - 1));
	switch ((int)hue / 60) {
	case 0:
		return PrecisePixel(M, z + m, m);
		break;
	case 1:
		return PrecisePixel(z + m, M, m);
		break;
	case 2:
		return PrecisePixel(m, M, z + m);
		break;
	case 3:
		return PrecisePixel(m, z + m, M);
		break;
	case 4:
		return PrecisePixel(z + m, m, M);
		break;
	case 5:
		return PrecisePixel(M, m, z + m);
		break;
	}
	
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
