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
	Kernel(int, std::string = "", double = 1);
	int getSize();
	float** getWeights();
	void setWeight(int, int, float);
	Pixel convolve(int, int, Image&);
	double convolve(int, int, ImageData&);

	static double* variance(Coord, ImageData&, ImageData&);
};

#include "Image.h"
#include "ImageData.h"
#include "Vec.h"