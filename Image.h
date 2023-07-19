#pragma once

#include "framework.h"
#include "Kernel.h"

class Image
{
private:
	int width;
	int height;
	Pixel* data;

public:
	Image(int, int);
	void WriteData(int, int, Pixel);
	char* GetData(int, int);
	Pixel GetPixel(int, int);
	void CopyTo(Image&);
	void ConvolveTo(Image&, Kernel&, int = 1);
	void ConvolveRows(Image&, Kernel&, int, int);
};

