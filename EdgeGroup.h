#pragma once

#include "framework.h"
#include "Node.h"
#include "ImageData.h"

class EdgeGroup
{
private:
	std::vector<Node*> points;
	std::vector<Coord> finalPoints;
	std::vector<double> slopes;
	std::vector<double> slopeDerivatives;
	double avgDirection;
	double variation;
	bool isCyclic;

public:

	EdgeGroup(int, int, ImageData&, ImageData&);

	void printData();
	int getSize();
	double getAvgDirection();
	double getVariation();
	std::vector<Node*> getPoints();
	std::vector<Coord> getFinalPoints();
	Node* getNode(int, int);
	
	Node* StemFrom(int, int, ImageData&, ImageData&);
	void TrimNodes(Node&);
	void TrimStem(Node&, ImageData&);
	void Order();
};

