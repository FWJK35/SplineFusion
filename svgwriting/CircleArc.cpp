#include "CircleArc.h"

/*
* Creates a new circle arc with given endpoints and radians of curvature
*/
CircleArc::CircleArc(Vec aStart, Vec aEnd, double aCurvature)
{
	start = aStart;
	end = aEnd;
	curvature = aCurvature;

	Vec mid = start.getMidPoint(end);
	double midDist = start.getDistance(mid);
	Vec perpendicular = (end - start).invert();
	perpendicular.setX(-perpendicular.getX());
	double factor = 0.5 / tan(curvature * 0.5);
	perpendicular = perpendicular * factor;
	center = mid + perpendicular;

	double perpDist = center.getDistance(mid);
	radius = std::sqrt(perpDist * perpDist + midDist * midDist);

	length = curvature * radius;
}

/*
* Draws this circle arc to a given image
*/
void CircleArc::DrawTo(Image& img)
{
	/*start.DrawTo(img);
	end.DrawTo(img);
	center.DrawTo(img);*/

	double angle = 0;
	double step = curvature / length;
	Vec startVec = start - center;
	for (int d = 0; d < length; d++) {
		double sinA = std::sin(angle);
		double cosA = std::cos(angle);
		
		double newX = startVec.getX() * cosA - startVec.getY() * sinA;
		double newY = startVec.getY() * cosA + startVec.getX() * sinA;

		Vec currentPoint(center + Vec(newX, newY));
		currentPoint.DrawTo(img);

		angle += step;
	}
}

/*
* Returns the center of a circle arc given two endpoints and any intermediate point
*/
Vec CircleArc::GetCenter(Vec endpoint1, Vec centerpoint, Vec endpoint2)
{
	if ((centerpoint - endpoint1).getSlope() == (centerpoint - endpoint2).getSlope()) {
		throw std::invalid_argument("Points cannot be colinear");
	}
	Vec diff1 = centerpoint - diff1;
	Vec diff2 = centerpoint - diff2;
	
	double xd1yd2 = diff1.getX() * diff2.getY(); 
	double xd2yd1 = diff2.getX() * diff2.getY(); 

	double centerX = ((diff1.getY() * diff2.getY() * (diff2.getY() - diff1.getY()) 
					+ xd1yd2 * endpoint1.getX() - xd2yd1 * endpoint2.getX()) 
					/ (xd1yd2 - xd2yd1) + centerpoint.getX()) * 0.5;
	double centerY = ((diff1.getX() * diff2.getX() * (diff2.getX() - diff1.getX()) 
					+ xd2yd1 * endpoint1.getY() - xd1yd2 * endpoint2.getY()) 
					/ (xd2yd1 - xd1yd2) + centerpoint.getY()) * 0.5; 

    return Vec(centerX, centerY);
}
