#include "Node.h"

Node::Node(Coord c)
{
	location = c;
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
	if (!exists && !(location == (*n).location)) connections.push_back(n);
}

void Node::Disconnect(Node* n)
{
	std::vector<Node*>::iterator it = connections.begin();
	while (it != connections.end()) {
		if (location == (*n).getLocation()) {
			connections.erase(it);
			return;
		}
		it++;
	}
}

Coord Node::getLocation()
{
	return location;
}

std::vector<Node*> Node::getConnections()
{
	return connections;
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
