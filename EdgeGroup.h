#pragma once

#include "framework.h"
#include "Node.h"
#include "ImageData.h"

class EdgeGroup
{
private:
	std::vector<Node*> points;
	double avgDirection;
	double variation;

public:
	static double compareAngles(double, double);

	EdgeGroup(int, int, ImageData&, ImageData&);

	void printData();
	int getSize();
	double getAvgDirection();
	double getVariation();
	std::vector<Node*> getPoints();
	Node* getNode(int, int);
	
	Node* StemFrom(int, int, ImageData&, ImageData&);
	void Order();
};

