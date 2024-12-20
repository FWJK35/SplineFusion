#include "Pixel.h"

/*
* Creates a new 3-byte black pixel
*/
Pixel::Pixel()
{
	red = 0;
	green = 0;
	blue = 0;
}

/*
* Creates a new 3-byte pixel with given rgb values
*/
Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b)
{
	red = r;
	green = g;
	blue = b;
}

/*
* Creates a new 3-byte grayscale pixel with a given value
*/
Pixel::Pixel(uint8_t v)
{
	red = v;
	green = v;
	blue = v;
}

/*
* Gets the red channel of this pixel
*/
int Pixel::getRed()
{
	return red;
}

/*
* Gets the green channel of this pixel
*/
int Pixel::getGreen()
{
	return green;
}

/*
* Gets the blue channel of this pixel
*/
int Pixel::getBlue()
{
	return blue;
}

/*
* Gets the average of the three color channels of this pixel
*/
double Pixel::getAvg()
{
	return (red + green + blue) / 3.0;
}

/*
* Returns this pixel represented as a string
*/
std::string Pixel::toString()
{
	std::ostringstream output;
	output << "(" << (int) red << " " << (int) green << " " << (int) blue << ")";
	return output.str();
}

/*
* Sets all channel values of this pixel to the average of them
*/
void Pixel::makeGrayScale()
{
	int avg = (red + green + blue) / 3;
	red = avg;
	green = avg;
	blue = avg;
}

/*
* Increases the contrast by pushing each color channel further towards 0 or 255
*/
void Pixel::increaseConrast(double amount)
{
	red = ((((red / 255.0) - 0.5) * amount) + 0.5) * 255.0;
	green = ((((green / 255.0) - 0.5) * amount) + 0.5) * 255.0;
	blue = ((((blue / 255.0) - 0.5) * amount) + 0.5) * 255.0;
}

/*
* Sets a pixel to white if the average of the three color channels
* is above a threshold, otherwise sets to black
*/
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

/*
* Converts this pixel to an array of bytes
*/
char* Pixel::toBytes()
{
	char* pix = new char[3];
	pix[0] = red;
	pix[1] = green;
	pix[2] = blue;
	return pix;
}

/*
* Adds another pixel's rgb channels to this pixel's
*/
void Pixel::operator+=(Pixel other)
{
	int newRed = std::min(red + other.red, 255);
	int newGreen = std::min(green + other.green, 255);
	int newBlue = std::min(blue + other.blue, 255);
	red = newRed;
	green = newGreen;
	blue = newBlue;
}

/*
* Returns the average of this pixel with another
*/
Pixel Pixel::operator/(Pixel other)
{
	int newRed = (red + other.red) / 2;
	int newGreen = (green + other.green) / 2;
	int newBlue = (blue + other.blue) / 2;
	return Pixel(newRed, newGreen, newBlue);
}

/*
* Returns a precise pixel that is this pixel's rbg channels
* scaled up by a given factor
*/
PrecisePixel Pixel::operator*(const float factor)
{
	float newRed = red * factor;
	float newGreen = green * factor;
	float newBlue = blue * factor;

	return PrecisePixel(newRed, newGreen, newBlue);
}
