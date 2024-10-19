#pragma once

#include "framework.h"
#include "Vec.h"
#include "Coord.h"

class VecField
{
private:
	int width;
	int height;
	Vec* data;

public:
	VecField(int, int);
	void WriteData(int, int, Vec);
	void WriteData(Coord, Vec);
	void AddValue(int, int, Vec);
	void AddValue(Coord, Vec);
	Vec GetData(int, int);
	void CopyTo(VecField&);
};

