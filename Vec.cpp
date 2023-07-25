#include "Vec.h"

Vec::Vec()
{
	x = 0;
	y = 0;
}

Vec::Vec(double aX, double aY)
{
	x = aX;
	y = aY;
}

double Vec::getX()
{
	return x;
}

double Vec::getY()
{
	return y;
}

void Vec::setX(double newX)
{
	x = newX;
}

void Vec::setY(double newY)
{
	y = newY;
}

Vec Vec::round()
{
	return Vec(std::round(x), std::round(y));
}

Coord Vec::toCoord()
{
	return Coord((int) x, (int) y);
}

std::string Vec::toString()
{
	std::ostringstream out;
	out << "(" << x << ", " << y << ")";
	return out.str();
}

double Vec::getArea()
{
	return std::abs(x * y);
}

Vec Vec::getMidPoint(Vec other)
{
	return Vec((x + other.x) / 2, (y + other.y) / 2);
}

double Vec::getDistance(Vec other)
{
	double xDist = x - other.x;
	double yDist = y - other.y;
	return std::sqrt(xDist * xDist + yDist * yDist);
}

Vec Vec::invert()
{
	return Vec(y, x);
}

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

Vec Vec::operator+(Vec p)
{
	return Vec(x + p.x, y + p.y);
}

Vec Vec::operator-(Vec p)
{
	return Vec(x - p.x, y - p.y);
}

Vec Vec::operator*(double f)
{
	return Vec(x * f, y * f);
}

Vec Vec::operator/(double f)
{
	return Vec(x / f, y / f);
}

void Vec::operator+=(Vec p)
{
	x += p.x;
	y += p.y;
}

void Vec::operator-=(Vec p)
{
	x -= p.x;
	y -= p.y;
}
