#pragma once

#include "framework.h"
#include "Coord.h"

class Node
{
private:
	std::vector<Node*> connections;
	Coord location;
public:
	Node(Coord);
	void Connect(Node*);
	void Disconnect(Node*);

	Coord getLocation();
	std::vector<Node*> getConnections();
	int getNeighbors();

	void printData();
};

