#include "Point.h"

Point::Point()
{
	x = 0;
	y = 0;
}

Point::Point(double aX, double aY)
{
	x = aX;
	y = aY;
}

double Point::getX()
{
	return x;
}

double Point::getY()
{
	return y;
}

double Point::getArea()
{
	return std::abs(x * y);
}

Point Point::round()
{
	return Point(std::round(x), std::round(y));
}

Coord Point::toCoord()
{
	return Coord((int) x, (int) y);
}

std::string Point::toString()
{
	std::ostringstream out;
	out << "(" << x << ", " << y << ")";
	return out.str();
}

void Point::DrawTo(Image& img)
{
	Point tr = *this + Point(0.5, -0.5);
	Point trc = tr.round();
	double trVal = (tr - (trc - Point(0.5, -0.5))).getArea() * 255;
	img.AddValue(trc.toCoord(), Pixel(trVal));

	Point br = *this + Point(0.5, 0.5);
	Point brc = br.round();
	double brVal = (br - (brc - Point(0.5, 0.5))).getArea() * 255;
	img.AddValue(brc.toCoord(), Pixel(brVal));

	Point bl = *this + Point(-0.5, 0.5);
	Point blc = bl.round();
	double blVal = (bl - (blc - Point(-0.5, 0.5))).getArea() * 255;
	img.AddValue(blc.toCoord(), Pixel(blVal));

	Point tl = *this + Point(-0.5, -0.5);
	Point tlc = tl.round();
	double tlVal = (tl - (tlc - Point(-0.5, -0.5))).getArea() * 255;
	img.AddValue(tlc.toCoord(), Pixel(tlVal));



}

Point Point::operator+(Point p)
{
	return Point(x + p.x, y + p.y);
}

Point Point::operator-(Point p)
{
	return Point(x - p.x, y - p.y);
}

Point Point::operator*(double f)
{
	return Point(x * f, y * f);
}

Point Point::operator/(double f)
{
	return Point(x / f, y / f);
}

void Point::operator+=(Point p)
{
	x += p.x;
	y += p.y;
}

void Point::operator-=(Point p)
{
	x -= p.x;
	y -= p.y;
}
