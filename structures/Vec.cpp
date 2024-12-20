#include "Vec.h"

/*
* Creates a new zero vector
*/
Vec::Vec()
{
	x = 0;
	y = 0;
}

/*
* Creates a new vector from given x and y values
*/
Vec::Vec(double aX, double aY)
{
	x = aX;
	y = aY;
}

/*
* Gets the x component of this vector
*/
double Vec::getX()
{
	return x;
}

/*
* Gets the y component of this vector
*/
double Vec::getY()
{
	return y;
}

/*
* Sets the x component of this vector to a given value
*/
void Vec::setX(double newX)
{
	x = newX;
}

/*
* Sets the y component of this vector to a given value
*/
void Vec::setY(double newY)
{
	y = newY;
}

/*
* Returns a new vector with x and y components rounded to the nearest integer
*/
Vec Vec::round()
{
	return Vec(std::round(x), std::round(y));
}

/*
* Returns this vector converted to a Coord by truncating any decimals
*/
Coord Vec::toCoord()
{
	return Coord((int) x, (int) y);
}

/*
* Returns this vector represented as a string
*/
std::string Vec::toString()
{
	std::ostringstream out;
	out << "(" << x << ", " << y << ")";
	return out.str();
}

/*
* Returns the area of the rectangle with this vector as the diagonal
*/
double Vec::getArea()
{
	return std::abs(x * y);
}

/*
* Returns the slope of this vector
*/
double Vec::getSlope()
{
	return y / x;
}

/*
* Returns the vector linearly interpolated between this one
* and another given vector a given distance between them
*/
Vec Vec::interpolate(Vec to, double dist)
{
	Vec diff = to - *this;
	return *this + (diff * dist);
}

/*
* Returns the midpoint of this vector and another given vector
*/
Vec Vec::getMidPoint(Vec other)
{
	return (*this + other) / 2;
}

/*
* Returns the distance between the two points represented by 
* this vector and another given vector
*/
double Vec::getDistance(Vec other)
{
	double xDist = x - other.x;
	double yDist = y - other.y;
	return std::sqrt(xDist * xDist + yDist * yDist);
}

/*
* Returns this vector with the x and y components swapped
*/
Vec Vec::invert()
{
	return Vec(y, x);
}

/*
* Draws this vector as a point to a given image
*/
void Vec::DrawTo(Image& img)
{
	Vec tr = *this + Vec(0.5, -0.5);
	Vec trc = tr.round();
	double trVal = (tr - (trc - Vec(0.5, -0.5))).getArea() * 255;
	img.AddValue(trc.toCoord(), Pixel(trVal));

	Vec br = *this + Vec(0.5, 0.5);
	Vec brc = br.round();
	double brVal = (br - (brc - Vec(0.5, 0.5))).getArea() * 255;
	img.AddValue(brc.toCoord(), Pixel(brVal));

	Vec bl = *this + Vec(-0.5, 0.5);
	Vec blc = bl.round();
	double blVal = (bl - (blc - Vec(-0.5, 0.5))).getArea() * 255;
	img.AddValue(blc.toCoord(), Pixel(blVal));

	Vec tl = *this + Vec(-0.5, -0.5);
	Vec tlc = tl.round();
	double tlVal = (tl - (tlc - Vec(-0.5, -0.5))).getArea() * 255;
	img.AddValue(tlc.toCoord(), Pixel(tlVal));
}

/*
* Returns this vector added to another given vector
*/
Vec Vec::operator+(Vec p)
{
	return Vec(x + p.x, y + p.y);
}

/*
* Returns this vector subtracted by another given vector
*/
Vec Vec::operator-(Vec p)
{
	return Vec(x - p.x, y - p.y);
}

/*
* Returns this vector scaled up by a given scaling factor
*/
Vec Vec::operator*(double f)
{
	return Vec(x * f, y * f);
}

/*
* Returns this vector scaled up by a given scaling factor
*/
Vec Vec::operator/(double f)
{
	return Vec(x / f, y / f);
}

/*
* Adds another given vector to this vector
*/
void Vec::operator+=(Vec p)
{
	x += p.x;
	y += p.y;
}

/*
* Subtracts another given vector from this vector
*/
void Vec::operator-=(Vec p)
{
	x -= p.x;
	y -= p.y;
}
