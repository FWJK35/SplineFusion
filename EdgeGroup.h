#pragma once

#include "framework.h"
#include "Node.h"
#include "ImageData.h"
#include "shapes.h"

class EdgeGroup
{
private:
	std::vector<Node*> points;
	std::vector<Coord> coords;
	std::vector<double> slopes;
	std::vector<double> calculatedSlopes;
	std::vector<double> slopeDerivatives;
	std::vector<double> slope2ndDerivatives;
	int size;
	bool isCyclic;

public:

	EdgeGroup(int, int, ImageData&, ImageData&);
	EdgeGroup();

	void printData();
	int getSize();
	bool getCyclic();
	std::vector<Node*> getPoints();
	std::vector<Coord> getFinalPoints();
	std::vector<double> getSlopes();
	std::vector<double> getCalculatedSlopes();
	std::vector<double> getSlopeDerivatives();
	std::vector<double> getSlope2ndDerivatives();
	Node* getNode(int, int);
	void addPoint(Coord, double);
	
	void StemFrom(int, int, ImageData&, ImageData&);
	void TrimNodes(Node&);
	void TrimStem(Node&, ImageData&);
	void Order();
	void CalculateSlopeData();
	int GetSplitLocation();
	int GetSubLine(Line&);
	int GetSubArc(CircleArc&);
	EdgeGroup* Split(int);
};

