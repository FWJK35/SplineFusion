#include "Helpers.h"

double compareAngles(double a, double b)
{
    double temp = std::min(a, b);
    b = std::max(a, b);
    a = temp;
    return std::min(b - a, a - b + M_PI);
}

double compareSignedAngles(double a, double b)
{
    double temp = b - a;
    if (temp > M_PI_2) temp -= M_PI;
    if (temp < -M_PI_2) temp += M_PI;
    return temp;
}

double compareFullAngles(double a, double b)
{
    double temp = std::min(a, b);
    b = std::max(a, b);
    a = temp;
    return std::min(b - a, a - b + M_PI * 2);
}

double limitedAtan(double a)
{
    double ans = std::atan(a);
    if (ans < 0) ans += M_PI;
    return ans;
}

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

double coordAtan(Coord c) {
    return atan((double) c.getY() / c.getX());
}