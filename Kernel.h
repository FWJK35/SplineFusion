#pragma once
#include "framework.h"
#include "Pixel.h"
class Image;

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
};

#include "Image.h"