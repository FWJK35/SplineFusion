#include "Node.h"

Node::Node(Coord c, double angle)
{
	location = c;
	myAngle = angle;
	bestNeighbors = 0;
	connections = std::vector<Node*>();
}

void Node::Connect(Node* n)
{
	if (n == nullptr) {
		return;
	} 
	bool exists = false;
	for (Node* t : connections) {
		if ((*t).location == (*n).location) exists = true;
	}
	if (!exists && !(location == (*n).location)) {
		connections.push_back(n);
	}
}

void Node::Disconnect(Node* n)
{
	std::vector<Node*>::iterator it = connections.begin();
	while (it != connections.end()) {
		if ((**it).getLocation() == (*n).getLocation()) {
			connections.erase(it);
			return;
		}
		it++;
	}
}

void Node::Kill()
{
	for (Node* cPointer : connections) {
		(*cPointer).Disconnect(this);
	}
}

void Node::addBestNeighbor()
{
	bestNeighbors++;
}

void Node::addBestNeighbor(int i)
{
	bestNeighbors += i;
}

void Node::setBestNeighbors()
{
	if (location == Coord(421, 23)) {
		int a = 0;
	}
	double minDiffA = M_PI;
	Node* pointerA = nullptr;
	double minDiffB = M_PI;
	Node* pointerB = nullptr;
	for (Node* cPointer : connections) {
		Node& c = *cPointer;
		Coord diff = c.getLocation() - location;
		//double angleDiff = compareAngles(0, 0);
		double angleDiff = compareAngles(c.getAngle(), std::atan((double)-diff.getY() / diff.getX()));
		if (angleDiff < minDiffA) {
			//shift A to B
			pointerB = pointerA;
			minDiffB = minDiffA;
			//make A new lowest diff
			pointerA = cPointer;
			minDiffA = angleDiff;
		}
		else if (angleDiff < minDiffB) {
			//make B new second lowest diff
			pointerB = cPointer;
			minDiffB = angleDiff;
		}
	}
	if (pointerA != nullptr) {
		(*pointerA).addBestNeighbor();
	}
	if (pointerB != nullptr) {
		(*pointerB).addBestNeighbor();
	}
}

Node::~Node()
{
	connections.clear();
	connections.shrink_to_fit();
}

Coord Node::getLocation()
{
	return location;
}

double Node::getAngle()
{
	return myAngle;
}

std::vector<Node*> Node::getConnections()
{
	return connections;
}

int Node::getBestNeighbors()
{
    return bestNeighbors;
}
int Node::getNeighbors()
{
	return connections.size();
}

void Node::printData()
{
	std::cout << location.toString() << ": ";
	for (Node* n : connections) {
		std::cout << (*n).location.toString() << ", ";
	}
	std::cout << std::endl;
}
