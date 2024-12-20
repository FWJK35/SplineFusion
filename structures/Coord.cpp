#include "Coord.h"

/*
* Default constructor for a new Coord
*/
Coord::Coord()
{
    x = 0;
    y = 0;
}

/*
* Constructs a Coord with given X and Y values
*/
Coord::Coord(int aX, int aY)
{
    x = aX;
    y = aY;
}

/*
* Gets the X value of the Coord
*/
int Coord::getX()
{
    return x;
}

/*
* Gets the Y value of the Coord
*/
int Coord::getY()
{
    return y;
}

/*
* Restricts the X and Y values to positive integers
*/
void Coord::abs()
{
    x = std::abs(x);
    y = std::abs(y);
}

/*
* Returns the string representation of this Coord
*/
std::string Coord::toString()
{
    std::ostringstream out;
    out << "(" << x << ", " << y << ")";
    return out.str();
}

/*
* Returns the magnitude of this Coord from (0, 0)
*/
double Coord::magnitude()
{
    return std::sqrt(x*x + y*y);
}

/*
* Returns the magnitude squared of this Coord from (0, 0)
*/
double Coord::magnitudeSquared()
{
    return (x * x + y * y);
}

/*
* Returns a Vec (double) representation of this Coord
*/
Vec Coord::toVec()
{
    return Vec(x, y);
}

/*
* Adds this Coord to another Coord
*/
Coord Coord::operator+(Coord c)
{
    return Coord(x + c.x, y + c.y);
}

/*
* Subtracts another Coord from this Coord
*/
Coord Coord::operator-(Coord c)
{
    return Coord(x - c.x, y - c.y);
}

/*
* Scales this Coord by an int magnitude
*/
Coord Coord::operator*(int i)
{
    return Coord(x * i, y * i);
}

/*
* Adds another Coord to this Coord
*/
void Coord::operator+=(Coord c)
{
    x += c.x;
    y += c.y;
}

/*
* Subtracts another Coord from this Coord
*/
void Coord::operator-=(Coord c)
{
    x -= c.x;
    y -= c.y;
}

/*
* Scales this Coord by an int magnitude
*/
void Coord::operator*=(int i) {
    x *= i;
    y *= i;
}

/*
* Checks if this Coord is equal to another Coord
*/
bool Coord::operator==(Coord c)
{
    return (x == c.x && y == c.y);
}
