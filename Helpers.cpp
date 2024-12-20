#include "Helpers.h"

/*
* Returns the absolute difference between two angles between 0 and pi in radians
*/
double compareAngles(double a, double b)
{
    double temp = std::min(a, b);
    b = std::max(a, b);
    a = temp;
    return std::min(b - a, a - b + M_PI);
}

/*
* Returns the signed difference between two angles between 0 and pi in radians
*/
double compareSignedAngles(double a, double b)
{
    double temp = b - a;
    if (temp > M_PI_2) temp -= M_PI;
    if (temp < -M_PI_2) temp += M_PI;
    return temp;
}

/*
* Returns the difference between two angles between 0 and 2pi radians
*/
double compareFullAngles(double a, double b)
{
    double temp = std::min(a, b);
    b = std::max(a, b);
    a = temp;
    return std::min(b - a, a - b + M_PI * 2);
}

/*
* Returns the angle of a given y/x ratio, limited in the range 0 to pi/2
*/
double limitedAtan(double a)
{
    double ans = std::atan(a);
    if (ans < 0) ans += M_PI;
    return ans;
}

/*
* Returns the angle of a given x and y offset, limited to increments of pi/4
*/
double fastAtan(int x, int y) {
    auto error = std::invalid_argument("Bad angle");
    switch (x) {
        case -1:
            switch (y) {
                case -1:
                return M_PI * 0.75;
                case 0:
                return M_PI * 1;
                case 1:
                return M_PI * 1.25;
                default:
                throw error;
            }
        break;
        case 0:
            switch (y) {
                    case -1:
                    return M_PI * 0.5;
                    case 0:
                    throw error;
                    case 1:
                    return M_PI * 1.5;
                    default:
                    throw error;
                }
        break;
        case 1:
            switch (y) {
                case -1:
                return M_PI * 0.25;
                case 0:
                return M_PI * 0;
                case 1:
                return M_PI * 1.75;
                default:
                throw error;
            }
        break;
        default:
        throw error;
    }
}

/*
* Returns the angle of a given Coord from the origin in the range 0 to pi
*/
double coordAtan(Coord c) {
    double result = atan((double) -c.getY() / c.getX());
    if (result < 0) result += M_PI;
    return result;
}