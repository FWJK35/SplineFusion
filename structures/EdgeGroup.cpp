#include "EdgeGroup.h"

/*
* Creates a new EdgeGroup, generating and connecting edges from
* a given start position, connecting correlated edges based on 
* the angle of the edge they represent.
*/
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

/*
* Creates a new empty EdgeGroup literally why would u ever use this
*/
EdgeGroup::EdgeGroup()
{
	size = 0;
	isCyclic = false;
}

/*
* Prints the coordinates of every point in this EdgeGroup
*/
void EdgeGroup::printData()
{
	for (Node* n : points) {
		Coord c = (*n).getLocation();
		std::cout << c.getX() << " " << c.getY() << std::endl;
	}
}

/*
* Gets the size of this EdgeGroup
*/
int EdgeGroup::getSize()
{
	return coords.size();
}

/*
* Gets whether this EdgeGroup is considered cyclic
*/
bool EdgeGroup::getCyclic()
{
	return isCyclic;
}

/*
* Gets the vector containing all initial 
* unordered nodes in this EdgeGroup
*/
std::vector<Node*> EdgeGroup::getPoints()
{
	return points;
}

/*
* Gets the vector containing all ordered 
* coordinate positions of nodes in this EdgeGroup
*/
std::vector<Coord> EdgeGroup::getFinalPoints()
{
    return coords;
}

/*
* Gets the vector containing all ordered edge slopes
* of the nodes in this EdgeGroup
*/
std::vector<double> EdgeGroup::getSlopes()
{
	return slopes;
}

/*
* Gets the vector containing all ordered calculated edge slopes
* based of weighted averages of adjacent nodes of the nodes in this EdgeGroup
*/
std::vector<double> EdgeGroup::getCalculatedSlopes()
{
	return calculatedSlopes;
}

/*
* Gets the vector containing all orderd calculated first derivatives between
* adjacent slopes of adjacent nodes in this EdgeGroup
*/
std::vector<double> EdgeGroup::getSlopeDerivatives()
{
	return slopeDerivatives;
}

/*
* Gets the vector containing all orderd calculated second derivatives between
* adjacent slopes of adjacent nodes in this EdgeGroup
*/
std::vector<double> EdgeGroup::getSlope2ndDerivatives()
{
	return slope2ndDerivatives;
}

/*
* Returns a single node in this EdgeGroup at the given x and y position,
* if it exists, otherwise returns nullptr
*/
Node* EdgeGroup::getNode(int x, int y)
{
	for (Node* n : points) {
		if ((*n).getLocation() == Coord(x, y)) return n;
	}
	return nullptr;
}

/*
* Adds a single point to the end of the coordinate and slope 
* vectors of this EdgeGroup
*/
void EdgeGroup::addPoint(Coord c, double angle)
{
	coords.push_back(c);
	slopes.push_back(angle);
	size++;
}

/*
* Recursive method to find all unfound adjacent nodes with 
* relatively similar angles to a given stem, adding them to 
* the list of found nodes in this EdgeGroup and setting them as "used"
*/
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

/*
* Trims nodes from a given node, removing as many connections
* as needed until only the two best matching connections remain
*/
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

/*
* Recursively trims a given node down to two connections, 
* makes that node immune to being trimmed, then continues 
* on to TrimStem any connecting nodes
*/
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

/*
* Puts all found nodes into an ordered vector of coordinates
* and an ordered vector of angles, creating a path for 
* future methods to follow and perform analysis on
*/
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

/*
* gets sum of distances of all intermediate points to the 
* secant line formed by the start and end points using precomputed sums
* sums in order are #p, x, x^2, y, y^2, x*y
*/
double EdgeGroup::GetDistSquaredSums(int startIndex, int endIndex, double* precomputedSums)
{

	Coord startPoint = coords[startIndex];
	Coord startEndDiff = coords[endIndex] - coords[startIndex];

	double xSumTerm1 = startPoint.getX() * startPoint.getX() * precomputedSums[0];
	double xSumTerm2 = 2 * startPoint.getX() * precomputedSums[1];
	double xSumTerm3 = precomputedSums[2];
	double xSum = startEndDiff.getY() * startEndDiff.getY() * (xSumTerm1 + xSumTerm2 + xSumTerm3);

	double ySumTerm1 = startPoint.getY() * startPoint.getY() * precomputedSums[0];
	double ySumTerm2 = 2 * startPoint.getY() * precomputedSums[3];
	double ySumTerm3 = precomputedSums[4];
	double ySum = startEndDiff.getX() * startEndDiff.getX() * (ySumTerm1 + ySumTerm2 + ySumTerm3);

	double xySumTerm1 = startPoint.getX() * startPoint.getY() * precomputedSums[0];
	double xySumTerm2 = precomputedSums[5];
	double xySumTerm3 = startPoint.getX() * precomputedSums[3];
	double xySumTerm4 = startPoint.getY() * precomputedSums[1];
	double xySum = 2 * startEndDiff.getX() * startEndDiff.getY() * (xySumTerm1 + xySumTerm2 - xySumTerm3 - xySumTerm4);

	double summedSquares = (xSum + ySum - xySum) / (startEndDiff.magnitudeSquared());

	return summedSquares;
}

/*
* TODO finish this lmao
* Gets the longest/first line of a desired length from this EdgeGroup
* but lowkey doesn't work :sob:
*/
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

/*
* Same thing as GetSubLine but I haven't even started lmao
*/
int EdgeGroup::GetSubArc(CircleArc& output) {
	return 0;
}

/*
* Gets the index of first occurring change in straightness of this EdgeGroup
*/
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

/*
* Splits this EdgeGroup at a given location and returns
* a pointer to the second EdgeGroup created. If this EdgeGroup
* was cyclic, makes itself non-cyclic and returns nullptr
*/
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
