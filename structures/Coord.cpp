#include "Coord.h"

Coord::Coord()
{
    x = 0;
    y = 0;
}

Coord::Coord(int aX, int aY)
{
    x = aX;
    y = aY;
}

int Coord::getX()
{
    return x;
}

int Coord::getY()
{
    return y;
}

void Coord::abs()
{
    x = std::abs(x);
    y = std::abs(y);
}

std::string Coord::toString()
{
    std::ostringstream out;
    out << "(" << x << ", " << y << ")";
    return out.str();
}

double Coord::magnitude()
{
    return std::sqrt(x*x + y*y);
}

double Coord::magnitudeSquared()
{
    return (x * x + y * y);
}

Vec Coord::toVec()
{
    return Vec(x, y);
}

Coord Coord::operator+(Coord c)
{
    return Coord(x + c.x, y + c.y);
}

Coord Coord::operator-(Coord c)
{
    return Coord(x - c.x, y - c.y);
}

Coord Coord::operator*(int i)
{
    return Coord(x * i, y * i);
}

void Coord::operator+=(Coord c)
{
    x += c.x;
    y += c.y;
}

void Coord::operator-=(Coord c)
{
    x -= c.x;
    y -= c.y;
}

bool Coord::operator==(Coord c)
{
    return (x == c.x && y == c.y);
}
