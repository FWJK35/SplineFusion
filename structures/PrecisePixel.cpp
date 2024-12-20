#include "PrecisePixel.h"

/*
* Creates a new floating point pixel with given rgb values
*/
PrecisePixel::PrecisePixel(float r, float g, float b)
{
	red = r;
	green = g;
	blue = b;
}

/*
* Gets the red channel of this pixel
*/
float PrecisePixel::getRed()
{
	return red;
}

/*
* Gets the green channel of this pixel
*/
float PrecisePixel::getGreen()
{
	return green;
}

/*
* Gets the blue channel of this pixel
*/
float PrecisePixel::getBlue()
{
	return blue;
}

/*
* Gets the average of the three color channels of this pixel
*/
double PrecisePixel::getAvg()
{
	return (red + green + blue) / 3.0;
}

/*
* Rounds this pixel to a 3-byte pixel
*/
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

/*
* Returns this pixel represented as a string
*/
std::string PrecisePixel::toString()
{
	std::ostringstream output;
	output << "(" << red << " " << green << " " << blue << ")";
	return output.str();
}

/*
* Returns a precise pixel that is this pixel's rbg channels
* scaled up by a given factor
*/
PrecisePixel PrecisePixel::operator*(const float factor)
{
	float newRed = red * factor;
	float newGreen = green * factor;
	float newBlue = blue * factor;

	return PrecisePixel(newRed, newGreen, newBlue);
}

/*
* Adds another pixel's rgb channels to this pixel's
*/
void PrecisePixel::operator+=(PrecisePixel added)
{
	red += added.red;
	green += added.green;
	blue += added.blue;
}

/*
* Returns a precise pixel with channels in HSV format
* from given rgb color channels
*/
PrecisePixel PrecisePixel::fromRGB(double R, double G, double B)
{
	double M = std::max(std::max(R, G), B);
	double m = std::min(std::min(R, G), B);
	double val = M / 255;
	double sat;
	if (M == 0) sat = 0;
	else sat = 1 - m / M;
	double hue;
	if (B > G) {
		hue = M_PI * 2 - std::acos((R - 0.5 * G - 0.5 * B) / std::sqrt(R*R + G*G + B*B - R*G - R*B - G*B));
	}
	else {
		hue = std::acos((R - 0.5 * G - 0.5 * B) / std::sqrt(R * R + G * G + B * B - R * G - R * B - G * B));
	}
	hue *= M_1_PI * 180;
	return PrecisePixel(hue, sat, val);
}

/*
* Returns a precise pixel with channels in HSV format
* from a given pixel with channels in rgb format
*/
PrecisePixel PrecisePixel::fromRGB(PrecisePixel p)
{
	return fromRGB(p.getRed(), p.getGreen(), p.getBlue());
}

/*
* Returns a precise pixel with channels in rgb format
* from given HSV color channels
*/
PrecisePixel PrecisePixel::fromHSV(double hue, double sat, double val)
{
	while (hue >= 360) hue -= 360;
	while (hue < 0) hue += 360;
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
	default:
		return PrecisePixel(0, 0, 0);
	}

}

/*
* Returns a precise pixel with channels in rgb format
* from a given pixel with channels in HSV format
*/
PrecisePixel PrecisePixel::fromHSV(PrecisePixel p)
{
	return fromHSV(p.getRed(), p.getGreen(), p.getBlue());
}
