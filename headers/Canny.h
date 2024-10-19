#pragma once
#include "framework.h"

#include "Image.h"
#include "ImageData.h"
#include "VecField.h"

class Canny
{
public:
	static void Sobel(Image& img, ImageData& directions, ImageData& magnitudes, VecField& output);
	static void Laplacian(Image& img, ImageData& directions, ImageData& magnitudes, ImageData& output);
	static void Threshold(ImageData& edgePixels, ImageData& magnitudes, double lower, double upper, ImageData& output);
	static void ConnectThreshold(ImageData& input, int x, int y);
};

