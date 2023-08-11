#pragma once

#include "framework.h"
#include "Coord.h"

class ImageData
{
private:
	int width;
	int height;
	double* data;

public:
	ImageData(int, int);
	void WriteData(int, int, double);
	void WriteData(Coord, double);
	void AddValue(int, int, double);
	void AddValue(Coord, double);
	double GetData(int, int);
	void CopyTo(ImageData&);
};

