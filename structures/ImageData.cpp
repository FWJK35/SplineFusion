#include "ImageData.h"

/*
* Creates a new ImageData object with a given width and height
*/
ImageData::ImageData(int aWidth, int aHeight)
{
	width = aWidth;
	height = aHeight;
	data = new double[width * height];
}

/*
* Gets the width of this ImageData
*/
int ImageData::getWidth()
{
	return width;
}

/*
* Gets the height of this ImageData
*/
int ImageData::getHeight()
{
	return height;
}

/*
* Writes a new data value to a given x and y position
*/
void ImageData::WriteData(int x, int y, double newData)
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
* Writes a new data value to a given Coord position
*/
void ImageData::WriteData(Coord c, double newData)
{
	WriteData(c.getX(), c.getY(), newData);
}

/*
* Adds a number to the current data value at a givent x and y position
*/
void ImageData::AddValue(int x, int y, double toAdd)
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
* Adds a number to the current data value at a givent Coord position
*/
void ImageData::AddValue(Coord c, double toAdd)
{
	AddValue(c.getX(), c.getY(), toAdd);
}

/*
* Gets the data at a given x and y postion
*/
double ImageData::GetData(int x, int y)
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
* Gets the data at a given Coord postion
*/
double ImageData::GetData(Coord c)
{
	return GetData(c.getX(), c.getY());
}

/*
* Copies this ImageData to a new ImageData object
*/
void ImageData::CopyTo(ImageData& newImageData)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			newImageData.WriteData(x, y, GetData(x, y));
		}
	}
}
