#include "CircleArc.h"

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
