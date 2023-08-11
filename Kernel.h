#pragma once
#include "framework.h"
#include "Pixel.h"
//#include "Vec.h"

class Image;
class ImageData;
class Vec;

class Kernel {
private:
	int size;
	int halfSize;
	std::string type;
	float** weights;
public:
	Kernel(int, std::string = "");
	int getSize();
	float** getWeights();
	Pixel convolve(int, int, Image&);

	static Vec gradient(Coord, Image&);
	static double* variance(Coord, ImageData&, ImageData&);
};

#include "Image.h"
#include "ImageData.h"
#include "Vec.h"