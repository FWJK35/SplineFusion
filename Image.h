#pragma once

#include "framework.h"
#include "Kernel.h"
#include "Coord.h"

class Image
{
private:
	int width;
	int height;
	Pixel* data;

public:
	Image(int, int);
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

