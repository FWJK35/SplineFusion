#include "VecField.h"

/*
* Creates a new VecField with a given width and height
*/
VecField::VecField(int aWidth, int aHeight)
{
	width = aWidth;
	height = aHeight;
	data = new Vec[width * height];
}

/*
* Writes a given vector to a given x and y position
*/
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

/*
* Writes a given vector to a given Coord position
*/
void VecField::WriteData(Coord c, Vec newData)
{
	WriteData(c.getX(), c.getY(), newData);
}

/*
* Adds a given vector to a given x and y postion
*/
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

/*
* Adds a given vector to a given Coord postion
*/
void VecField::AddValue(Coord c, Vec toAdd)
{
	AddValue(c.getX(), c.getY(), toAdd);
}

/*
* Gets the vector at a given x and y position
*/
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

/*
* Copies the data from this VecField to another given VecField
*/
void VecField::CopyTo(VecField& newVecField)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			newVecField.WriteData(x, y, GetData(x, y));
		}
	}
}
