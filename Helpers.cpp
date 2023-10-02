#include "Helpers.h"

double compareAngles(double a, double b)
{
    double temp = std::min(a, b);
    b = std::max(a, b);
    a = temp;
    return std::min(b - a, a - b + M_PI);
}

double limitedAtan(double a)
{
    double ans = std::atan(a);
    if (ans < 0) ans += M_PI;
    return ans;
}