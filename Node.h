#pragma once

#include "framework.h"
#include "Helpers.h"
#include "Coord.h"

class EdgeGroup;

class Node
{
private:
	std::vector<Node*> connections;
	double myAngle;
	int bestNeighbors;
	Coord location;
public:
	Node(Coord, double);
	void Connect(Node*);
	void Disconnect(Node*);
	void Kill();
	void addBestNeighbor();
	void addBestNeighbor(int);
	void setBestNeighbors();
	~Node();

	Coord getLocation();
    double getAngle();
    std::vector<Node *> getConnections();
    int getNeighbors();
	int getBestNeighbors();

	void printData();
};

#include "EdgeGroup.h"