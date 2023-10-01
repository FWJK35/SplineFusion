#include "EdgeGroup.h"

EdgeGroup::EdgeGroup(int x, int y, ImageData& edges, ImageData& angles)
{
	//collect points
	StemFrom(x, y, edges, angles);
	//connect points to all adjacent ones
	for (Node* nPointer : points) {
		Node& n = *nPointer;
		//std::cout << std::endl << n.getLocation().toString();
		for (Node* cPointer : points) {
			Node& c = *cPointer;
			Coord diff = c.getLocation() - n.getLocation();
			//if node is adjacent and not same node
			if (diff.getX() >= -1 && diff.getX() <= 1 && diff.getY() >= -1 && diff.getY() <= 1
				&& !(n.getLocation() == c.getLocation())) {
				//connect if nodes direction is close and direction of travel is close
				if (compareAngles(angles.GetData(n.getLocation()), angles.GetData(c.getLocation())) < M_PI_4
					&& compareAngles(angles.GetData(n.getLocation()), std::atan((double)-diff.getY() / diff.getX())) < M_PI_4) {
					n.Connect(cPointer);
				}
			}
		}
		n.setBestNeighbors();
		//reset image pixels
		edges.WriteData(n.getLocation(), 1);
	}
	TrimStem(*points[0], edges);
	Order();

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

std::vector<Coord> EdgeGroup::getFinalPoints()
{
    return finalPoints;
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
	
	Node* current = new Node(Coord(x, y), angles.GetData(Coord(x, y)));
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
					if (compareAngles(angles.GetData(newX, newY), std::atan((double)-yOffset / xOffset)) < M_PI_2) {
						StemFrom(newX, newY, edges, angles);
					}
				}
			}
		}
	}
	points.push_back(current);
	return current;
}

void EdgeGroup::TrimNodes(Node& n)
{
	while (n.getNeighbors() > 2) {
		//disconnect worst ones
		int worstNeighbors = 8;
		double worstDiff = 0;
		Node* worstPointer = nullptr;

		for (Node* cPointer : n.getConnections()) {
			Node& c = *cPointer;
			if (c.getBestNeighbors() < worstNeighbors) {
				worstPointer = cPointer;
				worstNeighbors = c.getBestNeighbors();
				worstDiff = compareAngles(n.getAngle(), c.getAngle());
			}
			else if (c.getBestNeighbors() == worstNeighbors) {
				double cAngleDiff = compareAngles(n.getAngle(), c.getAngle());
				if (cAngleDiff > worstDiff) {
					worstPointer = cPointer;
					worstDiff = cAngleDiff;
				}
			}
		}
		n.Disconnect(worstPointer);
		(*worstPointer).Disconnect(&n);
	}
}

void EdgeGroup::TrimStem(Node& n, ImageData& edges)
{
	TrimNodes(n);
	edges.WriteData(n.getLocation(), 0.5);
	if (n.getConnections().size() >= 1) {
		Node& con = *(n.getConnections()[0]);
		if (edges.GetData(con.getLocation()) == 1) {
			TrimStem(con, edges);
		}
	}
	if (n.getConnections().size() >= 2) {
		Node& con = *(n.getConnections()[1]);
		if (edges.GetData(con.getLocation()) == 1) {
			TrimStem(con, edges);
		}
	}
}

void EdgeGroup::Order()
{
	if (points.size() <= 2) {
		//finalPoints.push_back()
	}
	Node& current = *points[0];
	Coord lastPos(-1, -1);
	//keep searching for end node
	while (current.getNeighbors() > 1) {
		Node& next = *current.getConnections()[0];
		if (next.getLocation() == lastPos) {
			next = *current.getConnections()[1];
		}
		lastPos = current.getLocation();
		current = next;
	}

	//end node located, put nodes into final ordering
	//add first and second point
	finalPoints.push_back(current.getLocation());
	if (points.size() < 2) {
		return;
	}
	current = *current.getConnections()[0];

	while (current.getNeighbors() > 1) {
		Node& next = *current.getConnections()[0];
		if (next.getLocation() == lastPos) {
			next = *current.getConnections()[1];
		}
		lastPos = current.getLocation();
		finalPoints.push_back(current.getLocation());
		current = next;
	}
	finalPoints.push_back(current.getLocation());
}
