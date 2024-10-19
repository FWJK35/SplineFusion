#pragma once

#include "framework.h"
#include "Kernel.h"
class Coord;
class Pixel;

class Image
{
private:
	int width;
	int height;
	Pixel* data;

public:
	Image(int, int);
	int getWidth();
	int getHeight();

	void WriteData(int, int, Pixel);
	void WriteData(Coord, Pixel);
	void AddValue(int, int, Pixel);
	void AddValue(Coord, Pixel);
	char* GetData(int, int);
	Pixel GetPixel(int, int);
	Pixel GetPixel(Coord);
	void CopyTo(Image&);
	void ConvolveTo(Image&, Kernel&, int = 1);
	void ConvolveRows(Image&, Kernel&, int, int);
	void GrayScale();
};

#include "Coord.h"
#include "Pixel.h"