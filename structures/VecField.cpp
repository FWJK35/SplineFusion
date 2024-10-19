#include "VecField.h"

VecField::VecField(int aWidth, int aHeight)
{
	width = aWidth;
	height = aHeight;
	data = new Vec[width * height];
}

void VecField::WriteData(int x, int y, Vec newData)
{
	if (x < 0 || y < 0 || x >= width || y >= height) {
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (x >= width) x = width - 1;
		if (y >= height) y = height - 1;
	}
	data[y * width + x] = newData;
}

void VecField::WriteData(Coord c, Vec newData)
{
	WriteData(c.getX(), c.getY(), newData);
}

void VecField::AddValue(int x, int y, Vec toAdd)
{
	if (x < 0 || y < 0 || x >= width || y >= height) {
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (x >= width) x = width - 1;
		if (y >= height) y = height - 1;
	}

	data[y * width + x] += toAdd;
}

void VecField::AddValue(Coord c, Vec toAdd)
{
	AddValue(c.getX(), c.getY(), toAdd);
}

Vec VecField::GetData(int x, int y)
{
	if (x < 0 || y < 0 || x >= width || y >= height) {
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (x >= width) x = width - 1;
		if (y >= height) y = height - 1;
	}
	return data[y * width + x];
}

void VecField::CopyTo(VecField& newVecField)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			newVecField.WriteData(x, y, GetData(x, y));
		}
	}
}
