#include "ImageData.h"

ImageData::ImageData(int aWidth, int aHeight)
{
	width = aWidth;
	height = aHeight;
	data = new double[width * height];
}

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

void ImageData::WriteData(Coord c, double newData)
{
	WriteData(c.getX(), c.getY(), newData);
}

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

void ImageData::AddValue(Coord c, double toAdd)
{
	AddValue(c.getX(), c.getY(), toAdd);
}

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

void ImageData::CopyTo(ImageData& newImageData)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			newImageData.WriteData(x, y, GetData(x, y));
		}
	}
}
