#include "Pixel.h"

Pixel::Pixel()
{
	red = 0;
	green = 0;
	blue = 0;
}

Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b)
{
	red = r;
	green = g;
	blue = b;
}

Pixel::Pixel(uint8_t v)
{
	red = v;
	green = v;
	blue = v;
}

int Pixel::getRed()
{
	return red;
}

int Pixel::getGreen()
{
	return green;
}

int Pixel::getBlue()
{
	return blue;
}

double Pixel::getAvg()
{
	return (red + green + blue) / 3.0;
}

std::string Pixel::toString()
{
	std::ostringstream output;
	output << "(" << (int) red << " " << (int) green << " " << (int) blue << ")";
	return output.str();
}

void Pixel::makeGrayScale()
{
	int avg = (red + green + blue) / 3;
	red = avg;
	green = avg;
	blue = avg;
}

void Pixel::increaseConrast(double amount)
{
	red = ((((red / 255.0) - 0.5) * amount) + 0.5) * 255.0;
	green = ((((green / 255.0) - 0.5) * amount) + 0.5) * 255.0;
	blue = ((((blue / 255.0) - 0.5) * amount) + 0.5) * 255.0;
}

void Pixel::threshold(int threshold)
{
	int avg = (red + green + blue) / 3;
	if (avg > threshold) {
		red = 255;
		green = 255;
		blue = 255;
	}
	else {
		red = 0;
		green = 0;
		blue = 0;
	}
}

char* Pixel::toBytes()
{
	char* pix = new char[3];
	pix[0] = red;
	pix[1] = green;
	pix[2] = blue;
	return pix;
}

void Pixel::operator+=(Pixel other)
{
	int newRed = std::min(red + other.red, 255);
	int newGreen = std::min(green + other.green, 255);
	int newBlue = std::min(blue + other.blue, 255);
	red = newRed;
	green = newGreen;
	blue = newBlue;
}

Pixel Pixel::operator/(Pixel other)
{
	int newRed = (red + other.red) / 2;
	int newGreen = (green + other.green) / 2;
	int newBlue = (blue + other.blue) / 2;
	return Pixel(newRed, newGreen, newBlue);
}

PrecisePixel Pixel::operator*(const float factor)
{
	float newRed = red * factor;
	float newGreen = green * factor;
	float newBlue = blue * factor;

	return PrecisePixel(newRed, newGreen, newBlue);
}
