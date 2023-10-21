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
	bool isCyclic;

public:

	EdgeGroup(int, int, ImageData&, ImageData&);
	EdgeGroup();

	void printData();
	int getSize();
	std::vector<Node*> getPoints();
	std::vector<Coord> getFinalPoints();
	std::vector<double> getSlopes();
	std::vector<double> getSlopeDerivatives();
	Node* getNode(int, int);
	void addPoint(int, int, double);
	
	void StemFrom(int, int, ImageData&, ImageData&);
	void TrimNodes(Node&);
	void TrimStem(Node&, ImageData&);
	void Order();
	void CalculateSlopeDerivatives();
	int GetSplitLocation();
};

