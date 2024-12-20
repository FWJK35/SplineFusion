#include "Spline.h"

/*
* Creates a cubic spline with four given control points
*/
Spline::Spline(Vec a, Vec b, Vec c, Vec d)
{
	start = a;
	controlA = b;
	controlB = c;
	end = d;

	double approxLen = 0;
	const int segments = 12;
	double step = 1.0 / segments;
	for (int d = 0; d < segments; d++) {
		approxLen += getPointAlong(d * step).getDistance(getPointAlong((d + 1) * step));
	}
	length = approxLen;
	std::cout << length << std::endl;

}

/*
* Gets a point a given dist along this spline
*/
Vec Spline::getPointAlong(double dist)
{
	Vec vec1 = start.interpolate(controlA, dist);
	Vec vec2 = controlA.interpolate(controlB, dist);
	Vec vec3 = controlB.interpolate(end, dist);

	Vec endp1 = vec1.interpolate(vec2, dist);
	Vec endp2 = vec2.interpolate(vec3, dist);

	return endp1.interpolate(endp2, dist);
}

/*
* Draws this spline to a given image
*/
void Spline::drawTo(Image& img)
{
	double step = 1.0 / length;
	for (int d = 0; d < length; d++) {
		//std::cout << d << " ";
		getPointAlong(d * step).DrawTo(img);
	}
}
