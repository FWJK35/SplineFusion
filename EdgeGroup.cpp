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
	for (Node* nPointer : points) {
		Node n = *nPointer;
		//std::cout << std::endl << n.getLocation().toString();
		for (Node* cPointer : points) {
			Node c = *cPointer;
			Coord diff = c.getLocation() - n.getLocation();
			if (diff.getX() >= -1 && diff.getX() <= 1 && diff.getY() >= -1 && diff.getY() <= 1
				&& !(n.getLocation() == c.getLocation())) {
				if (compareAngles(angles.GetData(n.getLocation()), angles.GetData(c.getLocation())) < M_PI_4
					&& compareAngles(angles.GetData(n.getLocation()), std::atan((double)-diff.getY() / diff.getX())) < M_PI_4) {
					(*nPointer).Connect(cPointer);
				}
			}
		}
	}

	double avgX = 0;
	double avgY = 0;
	for (Node* n : points) {
		Coord c = (*n).getLocation();
		avgX += cos(angles.GetData(c));
		avgY += sin(angles.GetData(c));
	}
	avgDirection = std::atan(avgY / avgX);
	if (avgDirection < 0) avgDirection += M_PI;

	variation = 0;
	for (Node* n : points) {
		Coord c = (*n).getLocation();
		variation += compareAngles(avgDirection, angles.GetData(c));
	}
	variation /= points.size();

}

void EdgeGroup::printData()
{
	for (Node* n : points) {
		Coord c = (*n).getLocation();
		std::cout << c.getX() << " " << c.getY() << std::endl;
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

std::vector<Node*> EdgeGroup::getPoints()
{
	return points;
}

Node* EdgeGroup::getNode(int x, int y)
{
	for (Node* n : points) {
		if ((*n).getLocation() == Coord(x, y)) return n;
	}
	return nullptr;
}

Node* EdgeGroup::StemFrom(int x, int y, ImageData& edges, ImageData& angles)
{
	
	Node* current = new Node(Coord(x, y));
	//set current pixel to "used" value, unable to be revisited
	edges.WriteData(x, y, 0.5);

	for (int yOffset = -1; yOffset <= 1; yOffset++) {
		int newY = y + yOffset;
		for (int xOffset = -1; xOffset <= 1; xOffset++) {
			int newX = x + xOffset;
			if (edges.GetData(newX, newY) >= 1) {
				//some angle similarity value
				if (compareAngles(angles.GetData(newX, newY), angles.GetData(x, y)) < M_PI_4) {
					//also check that angle direction is close enough to connection direction
					if (compareAngles(angles.GetData(newX, newY), std::atan((double)-yOffset / xOffset)) < M_PI_4) {
						StemFrom(newX, newY, edges, angles);
					}
				}
			}
		}
	}
	points.push_back(current);
	return current;
}

void EdgeGroup::Order()
{
	Coord start((*points[0]).getLocation());
	for (Node* n : points) {
		Coord c = (*n).getLocation();
		if (c.getY() < start.getY()) {
			start = Coord(c);
		}
		else if (c.getY() == start.getY() && c.getX() < start.getX()) {
			start = Coord(c);
		}
	}


}
