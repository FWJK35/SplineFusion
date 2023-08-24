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
