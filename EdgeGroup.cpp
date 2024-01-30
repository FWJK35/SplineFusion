#include "EdgeGroup.h"

EdgeGroup::EdgeGroup(int x, int y, ImageData& edges, ImageData& angles)
{
	//collect points
	StemFrom(x, y, edges, angles);
	//connect points to all adjacent ones
	for (Node* nPointer : points) {
		Node& n = *nPointer;
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
	//free memory of nodes
	for (Node* n : points) {
		//delete(n);
	}
}

EdgeGroup::EdgeGroup()
{
	size = 0;
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
	return coords.size();
}

bool EdgeGroup::getCyclic()
{
	return isCyclic;
}

std::vector<Node*> EdgeGroup::getPoints()
{
	return points;
}

std::vector<Coord> EdgeGroup::getFinalPoints()
{
    return coords;
}

std::vector<double> EdgeGroup::getSlopes()
{
	return slopes;
}

std::vector<double> EdgeGroup::getCalculatedSlopes()
{
	return calculatedSlopes;
}

std::vector<double> EdgeGroup::getSlopeDerivatives()
{
	return slopeDerivatives;
}

std::vector<double> EdgeGroup::getSlope2ndDerivatives()
{
	return slope2ndDerivatives;
}

Node* EdgeGroup::getNode(int x, int y)
{
	for (Node* n : points) {
		if ((*n).getLocation() == Coord(x, y)) return n;
	}
	return nullptr;
}

void EdgeGroup::addPoint(Coord c, double angle)
{
	coords.push_back(c);
	slopes.push_back(angle);
	size++;
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
		coords.push_back((*points[0]).getLocation());
		slopes.push_back((*points[0]).getAngle());
		if (points.size() == 2) {
			coords.push_back((*points[1]).getLocation());
			slopes.push_back((*points[1]).getAngle());
		}
		size = coords.size();
		return;
	}
	Node current = *points[0];
	Node next = current;
	Coord lastPos = current.getLocation();
	current = *(current.getConnections()[0]);
	//keep searching for end node
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
	//end node located, put nodes into final ordering
	//add first point
	coords.push_back(current.getLocation());
	slopes.push_back(current.getAngle());
	lastPos = current.getLocation();
	current = *(current.getConnections()[0]);

	while (current.getNeighbors() > 1) {
		next = *(current.getConnections()[0]);
		if (next.getLocation() == lastPos) {
			next = *(current.getConnections()[1]);
		}
		if (next.getLocation() == coords[0]) {
			isCyclic = true;
			break;
		}
		lastPos = current.getLocation();
		coords.push_back(current.getLocation());
		slopes.push_back(current.getAngle());
		current = next;
	}
	coords.push_back(current.getLocation());
	slopes.push_back(current.getAngle());
	size = coords.size();
}

int EdgeGroup::GetSubLine(Line& output) 
{
	int minSize = 5;
	int startIndex = 0;
	bool startFound = false;

	//find first possible line of minimum size
	while (startIndex <= size - minSize) {
		double totalDist = 0;
		Coord ep1 = coords[startIndex];
		Coord ep2 = coords[startIndex + minSize];
		Coord endpointDiff = ep2 - ep1;
		//calculate how good of a fit the first minSize points are
		for (int p = 1; p < minSize - 1; p++) {
			Coord p0 = coords[startIndex + p];
			Coord pointDiff = ep1 - p0;
			//distance of that point from the line
			double dist = std::abs(endpointDiff.getX() * pointDiff.getY() - pointDiff.getX() * endpointDiff.getY()) / endpointDiff.magnitude();
			totalDist += dist;
		}
		//if that line is close enough
		if (totalDist / (minSize-2) < 0.5) {
			startFound = true;
			break;
		}
		startIndex++;
	}
	if (!startFound) {
		return -1;
	}

	//find the endpoint where the correlation gets too poor
	int endIndex = startIndex + minSize - 2;
	//note: lower correlation values = more accurate
	std::vector<double> correlationValues;

	while (endIndex < size) {
		double totalDist = 0;
		Coord ep1 = coords[startIndex];
		Coord ep2 = coords[endIndex];
		Coord endpointDiff = ep2 - ep1;
		double endpointDiffMagInverse = 1 / endpointDiff.magnitude();
		for (int p = startIndex + 1; p < endIndex; p++) {
			Coord p0 = coords[p];
			Coord pointDiff = ep1 - p0;
			//distance of that point from the line
			double dist = std::abs(endpointDiff.getX() * pointDiff.getY() - pointDiff.getX() * endpointDiff.getY()) * endpointDiffMagInverse;
			totalDist += dist;
		}
		double avgDist = totalDist / (endIndex - startIndex + 1);
		correlationValues.push_back(avgDist);
		if (avgDist > 0.5) {
			break;
		}
		endIndex++;
	}

	//find where the line actually stopped
	//correlation is higher / negative change
	while (endIndex >= startIndex + minSize - 1) {
		if (correlationValues[endIndex - startIndex - minSize + 2] -
			correlationValues[endIndex - startIndex - minSize + 1] < 0 ||
			correlationValues[endIndex - startIndex - minSize + 2] < 0.25) {
			break;
		}
		endIndex--;
	}

	Line result(coords[startIndex].toVec(), coords[endIndex].toVec());
	output = result;

	return 0;
}

int EdgeGroup::GetSubArc(CircleArc& output) {
	return 0;
}

int EdgeGroup::GetSplitLocation()
{
	return -1;
	//min split size
	int minSize = 4;
	if (slopes.size() < minSize) return -1;
	
	std::vector<double> correlationValues;
	//add filler correlation values;
	correlationValues.push_back(0);
	double maxCorrelation = 0;
	for (int i = 1; i < size; i++) {
		//calculate r-squared
		double totalDistSquared = 0;
		for (int p = 1; p < i; p++) {
			Coord p0 = coords[p];
			Coord p1 = coords[0];
			Coord p2 = coords[i];
			Coord endpointDiff = p2 - p1;
			Coord pointDiff = p1 - p0;
			double dist = (endpointDiff.getX() * pointDiff.getY() - pointDiff.getX() * endpointDiff.getY()) / endpointDiff.magnitude();
			totalDistSquared += dist * dist;
		}
		
		totalDistSquared /= i;
		if (totalDistSquared > maxCorrelation) {
			maxCorrelation = totalDistSquared;
		}
		correlationValues.push_back(totalDistSquared);
		
	}
	calculatedSlopes.clear();
	for (int i = 0; i < correlationValues.size(); i++) {
		double correlation = correlationValues[i];
		calculatedSlopes.push_back(correlation / maxCorrelation);
	}
	return -1;
}

EdgeGroup* EdgeGroup::Split(int splitLocation)
{
	EdgeGroup* other = new EdgeGroup();
	if (isCyclic) {
		isCyclic = false;
		for (int i = 0; i < splitLocation; i++) {
			for (int s = 0; s < size; s++) {
				coords.push_back(*coords.erase(coords.begin()));
				slopes.push_back(*slopes.erase(slopes.begin()));
			}
		}
		return nullptr;
	}
	auto pointsItr = coords.begin();
	auto slopesItr = slopes.begin();
	//pointsItr += splitLocation;
	//slopesItr += splitLocation;
	for (int i = 0; i < splitLocation; i++) {
		(*other).addPoint(coords[0], slopes[0]);
		pointsItr = coords.erase(pointsItr);
		slopesItr = slopes.erase(slopesItr);
	}
	size = coords.size();
	return other;
}
