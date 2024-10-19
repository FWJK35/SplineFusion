#include "Image.h"

Image::Image(int aWidth, int aHeight)
{
	width = aWidth;
	height = aHeight;
	data = new Pixel[width * height];
}

int Image::getWidth()
{
	return width;
}

int Image::getHeight()
{
	return height;
}

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

void Image::WriteData(Coord c, Pixel newData)
{
	WriteData(c.getX(), c.getY(), newData);
}

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

void Image::AddValue(Coord c, Pixel toAdd)
{
	AddValue(c.getX(), c.getY(), toAdd);
}

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

Pixel Image::GetPixel(Coord c)
{
	return GetPixel(c.getX(), c.getY());
}

void Image::CopyTo(Image& newImage)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			newImage.WriteData(x, y, GetPixel(x, y));
		}
	}
}

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

void Image::ConvolveRows(Image& toImg, Kernel& kernel, int from, int to)
{
	for (int y = from; y < to; y++) {
		for (int x = 0; x < width; x++) {
			Pixel convolved = kernel.convolve(x, y, *this);
			toImg.WriteData(x, y, convolved);
		}
	}
}

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
