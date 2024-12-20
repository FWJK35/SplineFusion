#include "Image.h"

/*
* Creates a new blank image with desired width and height
*/
Image::Image(int aWidth, int aHeight)
{
	width = aWidth;
	height = aHeight;
	data = new Pixel[width * height];
}

/* 
* Gets the width of this image
*/
int Image::getWidth()
{
	return width;
}

/*
* Gets the height of this image
*/
int Image::getHeight()
{
	return height;
}

/*
* Writes a single pixel of data to a given x and y position
*/
void Image::WriteData(int x, int y, Pixel newData)
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
* Writes a single pixel of data to a Coord position
*/
void Image::WriteData(Coord c, Pixel newData)
{
	WriteData(c.getX(), c.getY(), newData);
}

/*
* Adds a single pixel of data to a given x and y position
*/
void Image::AddValue(int x, int y, Pixel toAdd)
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
* Adds a single pixel of data to a given Coord position
*/
void Image::AddValue(Coord c, Pixel toAdd)
{
	AddValue(c.getX(), c.getY(), toAdd);
}

/*
* Returns the pixel data at a given x and y position
*/
char* Image::GetData(int x, int y)
{
	if (x < 0 || y < 0 || x >= width || y >= height) {
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (x >= width) x = width - 1;
		if (y >= height) y = height - 1;
	}
	return data[y * width + x].toBytes();
}

/*
* Returns the pixel object at a given x and y position
*/
Pixel Image::GetPixel(int x, int y)
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
* Returns the pixel object at a given Coord position
*/
Pixel Image::GetPixel(Coord c)
{
	return GetPixel(c.getX(), c.getY());
}

/*
* Copies the data from this image to a new image
*/
void Image::CopyTo(Image& newImage)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			newImage.WriteData(x, y, GetPixel(x, y));
		}
	}
}

/*
* Runs a convolution using a given Kernel on this image, 
* writing the data to a new image, using a given number of threads
*/
void Image::ConvolveTo(Image& toImg, Kernel& kernel, int threads)
{
	int blockSize = height / threads;
	Image secondImg = toImg;

	std::vector<std::thread> threadVector;
	for (int t = 0; t < threads; t++) {
		int from = blockSize * t;
		int to = blockSize * (t + 1);
		if (t == threads - 1) {
			to = height;
		}
		threadVector.push_back(std::thread(&Image::ConvolveRows, this, std::ref(toImg), std::ref(kernel), from, to));
	}
	for (int t = 0; t < threads; t++) {
		threadVector[t].join();
	}
}

/*
* Runs a convolution on a given range of rows,
* writing output data to a new image
*/
void Image::ConvolveRows(Image& toImg, Kernel& kernel, int from, int to)
{
	for (int y = from; y < to; y++) {
		for (int x = 0; x < width; x++) {
			Pixel convolved = kernel.convolve(x, y, *this);
			toImg.WriteData(x, y, convolved);
		}
	}
}

/*
* Converts this image to grayscale by averaging all pixel values
*/
void Image::GrayScale()
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Pixel current = GetPixel(x, y);
			int avg = (current.getRed() + current.getGreen() + current.getBlue()) / 3;
			WriteData(x, y, Pixel(avg, avg, avg));
		}
	}
}
