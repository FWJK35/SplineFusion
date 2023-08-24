#include "EdgeGroup.h"

double EdgeGroup::compareAngles(double a, double b)
{
    double temp = std::min(a, b);
    b = std::max(a, b);
    a = temp;
    return std::min(b - a, a - b + M_PI);
}

EdgeGroup::EdgeGroup(int x, int y, ImageData& edges, ImageData& angles)
{
	//collect points
	StemFrom(x, y, edges, angles);

	double avgX = 0;
	double avgY = 0;
	for (int p = 0; p < points.size(); p++) {
		avgX += cos(angles.GetData(points[p]));
		avgY += sin(angles.GetData(points[p]));
	}
	avgDirection = std::atan(avgY / avgX);
	if (avgDirection < 0) avgDirection += M_PI;

	variation = 0;
	for (int p = 0; p < points.size(); p++) {
		variation += compareAngles(avgDirection, angles.GetData(points[p]));
	}
	variation /= points.size();

}

void EdgeGroup::printData()
{
	for (int i = 0; i < getSize(); i++) {
		std::cout << points[i].getX() << " " << points[i].getY() << std::endl;
	}
}

int EdgeGroup::getSize()
{
	return points.size();
}

double EdgeGroup::getAvgDirection()
{
	return avgDirection;
}

double EdgeGroup::getVariation()
{
	return variation;
}

std::vector<Coord> EdgeGroup::getPoints()
{
	return points;
}

void EdgeGroup::StemFrom(int x, int y, ImageData& edges, ImageData& angles)
{
	//set current pixel to "used" value, unable to be revisited
	points.push_back(Coord(x, y));
	edges.WriteData(x, y, 0.5);

	for (int yOffset = -1; yOffset <= 1; yOffset++) {
		int newY = y + yOffset;
		for (int xOffset = -1; xOffset <= 1; xOffset++) {
			int newX = x + xOffset;
			if (edges.GetData(newX, newY) == 1) {
				//some angle similarity value
				if (compareAngles(angles.GetData(newX, newY), angles.GetData(x, y)) < M_PI_4) {
					//also check that angle direction is close enough to connection direction
					if (compareAngles(angles.GetData(newX, newY), std::atan((double)-yOffset / xOffset)) < M_PI_4)
					StemFrom(newX, newY, edges, angles);
				}
			}
		}
	}
}
