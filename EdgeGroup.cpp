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
					&& compareAngles(angles.GetData(n.getLocation()), limitedAtan((double)-diff.getY() / diff.getX())) < M_PI_4) {
					n.Connect(cPointer);
					c.Connect(nPointer);
				}
			}
		}
		
		//reset image pixels
		edges.WriteData(n.getLocation(), 1);
	}
	for (Node* nPointer : points) {
		(*nPointer).setBestNeighbors();
	}
	//limit nodes to have max of 2 neighbors, one in each direction
	TrimStem(*points[0], edges);
	isCyclic = false;
	//place nodes in order, starting from one end, or starting node if cyclic
	Order();
	CalculateSlopeDerivatives();
	//free memory of nodes
	for (Node* n : points) {
		//delete(n);
	}
}

EdgeGroup::EdgeGroup()
{
	isCyclic = false;
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
	return finalPoints.size();
}

std::vector<Node*> EdgeGroup::getPoints()
{
	return points;
}

std::vector<Coord> EdgeGroup::getFinalPoints()
{
    return finalPoints;
}

std::vector<double> EdgeGroup::getSlopes()
{
	return slopes;
}

std::vector<double> EdgeGroup::getSlopeDerivatives()
{
	return slopeDerivatives;
}

Node* EdgeGroup::getNode(int x, int y)
{
	for (Node* n : points) {
		if ((*n).getLocation() == Coord(x, y)) return n;
	}
	return nullptr;
}

void EdgeGroup::addPoint(int x, int y, double angle)
{
	finalPoints.push_back(Coord(x, y));
	slopes.push_back(angle);
}

void EdgeGroup::StemFrom(int x, int y, ImageData& edges, ImageData& angles)
{
	Node* current = new Node(Coord(x, y), angles.GetData(Coord(x, y)));
	points.push_back(current);
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
					if (compareAngles(angles.GetData(newX, newY), limitedAtan((double)-yOffset / xOffset)) < M_PI_4) {
						StemFrom(newX, newY, edges, angles);
					}
				}
			}
		}
	}
}

void EdgeGroup::TrimNodes(Node& n)
{
	int bestNeighbors1 = 0;
	double bestDiff1 = M_PI;
	Node* bestPointer1 = nullptr;
	int bestNeighbors2 = 0;
	double bestDiff2 = M_PI;
	Node* bestPointer2 = nullptr;
	for (Node* cPointer : n.getConnections()) {
		Node& c = *cPointer;
		Coord diff = c.getLocation() - n.getLocation();
		//determine left or right side
		if (compareFullAngles(fastAtan(diff.getX(), diff.getY()), n.getAngle()) <= M_PI_2) {
			//right side
			if (c.getBestNeighbors() > bestNeighbors1) {
				bestPointer1 = cPointer;
				bestNeighbors1 = c.getBestNeighbors();
				bestDiff1 = compareAngles(n.getAngle(), c.getAngle());
			}
			else if (c.getBestNeighbors() == bestNeighbors1) {
				double cAngleDiff = compareAngles(n.getAngle(), c.getAngle());
				if (cAngleDiff < bestDiff1) {
					bestPointer1 = cPointer;
					bestDiff1 = cAngleDiff;
				}
			}
		}
		else {
			//left side
			if (c.getBestNeighbors() > bestNeighbors2) {
				bestPointer2 = cPointer;
				bestNeighbors2 = c.getBestNeighbors();
				bestDiff2 = compareAngles(n.getAngle(), c.getAngle());
			}
			else if (c.getBestNeighbors() == bestNeighbors2) {
				double cAngleDiff = compareAngles(n.getAngle(), c.getAngle());
				if (cAngleDiff < bestDiff2) {
					bestPointer2 = cPointer;
					bestDiff2 = cAngleDiff;
				}
			}
		}
	}
	for (Node* cPointer : n.getConnections()) {
		if (cPointer != bestPointer1 && cPointer != bestPointer2) {
			(*cPointer).Kill();
		}
	}
}

void EdgeGroup::TrimStem(Node& n, ImageData& edges)
{
	TrimNodes(n);
	edges.WriteData(n.getLocation(), 0.5);
	//make self immune to trimming
	n.addBestNeighbor(8);
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
		finalPoints.push_back((*points[0]).getLocation());
		slopes.push_back((*points[0]).getAngle());
		if (points.size() == 2) {
			finalPoints.push_back((*points[1]).getLocation());
			slopes.push_back((*points[1]).getAngle());
		}
		return;
	}
	Node current = *points[0];
	Node next = current;
	Coord lastPos = current.getLocation();
	current = *(current.getConnections()[0]);
	//keep searching for end node
	std::cout << "searching for start...\n";
	current.printData();
	while (current.getNeighbors() > 1) {
		next = *(current.getConnections()[0]);
		if (next.getLocation() == lastPos) {
			next = *(current.getConnections()[1]);
		}
		if (next.getLocation() == (*points[0]).getLocation()) {
			current = next;
			break;
		}
		lastPos = current.getLocation();
		current = next;
	}
	std::cout << "starting point: ";
	current.printData();
	//end node located, put nodes into final ordering
	//add first point
	finalPoints.push_back(current.getLocation());
	slopes.push_back(current.getAngle());
	lastPos = current.getLocation();
	current = *(current.getConnections()[0]);

	while (current.getNeighbors() > 1) {
		next = *(current.getConnections()[0]);
		if (next.getLocation() == lastPos) {
			next = *(current.getConnections()[1]);
		}
		if (next.getLocation() == finalPoints[0]) {
			isCyclic = true;
			break;
		}
		lastPos = current.getLocation();
		finalPoints.push_back(current.getLocation());
		slopes.push_back(current.getAngle());
		current = next;
	}
	finalPoints.push_back(current.getLocation());
	slopes.push_back(current.getAngle());
}

void EdgeGroup::CalculateSlopeDerivatives()
{
	slopeDerivatives.push_back(0);
	for (int i = 1; i < slopes.size() - 1; i++) {
		double thisSlopeDerivative = compareAngles(slopes[i + 1], slopes[i - 1]) / 2;
		slopeDerivatives.push_back(thisSlopeDerivative);
	}
	slopeDerivatives.push_back(0);
}

int EdgeGroup::GetSplitLocation()
{
	for (int i = 0; i < slopes.size(); i++) {

	}
	return -1;
}
