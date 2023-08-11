#include "Kernel.h"

Kernel::Kernel(int aSize, std::string aType)
{
	size = aSize;
	halfSize = size / 2;
	type = aType;
	weights = new float* [size];
	for (int i = 0; i < size; i++) {
		weights[i] = new float[size];
		for (int j = 0; j < size; j++) {
			weights[i][j] = 0.0;
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (type == "blur") {
				weights[i][j] = 1.0f / (size * size);
			}
			else if (type == "edge") {
				if (i == 0 || j == 0 || i == size - 1 || j == size - 1) {
					weights[i][j] = 1.0f / (size * 4 - 4);
				}
				else if (i == halfSize && j == halfSize) {
					weights[i][j] = -1.0f;
				}
			}
			else if (type == "sobelh") {
				float a = halfSize - j;
				weights[i][j] = a * (halfSize + 1 - (std::abs(halfSize - i))) / (4);
				std::cout << weights[i][j] << " ";
			}
			else if (type == "sobelv") {
				float a = halfSize - i;
				weights[i][j] = a * (halfSize + 1 - (std::abs(halfSize - j))) / (4);
				std::cout << weights[i][j] << " ";
			}
			else if (type == "gaussblur") {
				double distx = (j - halfSize);
				double disty = (i - halfSize);
				double stddv = 1;
				double calc = 1.0 / (2 * M_PI * stddv * stddv) * pow(M_E,  (distx * distx + disty * disty) / -2 * (stddv * stddv));
				weights[i][j] = calc;
				std::cout << calc << " ";
			}
			else if (type == "focusblur") {
				if (i == 0 || j == 0 || i == size - 1 || j == size - 1) {
					weights[i][j] = -1.0f / (size * 4 - 4);
				}
			}
			else {
				if (i == halfSize && j == halfSize) {
					weights[i][j] = 2.0;
				}
			}
			
			
		}
	}
}

int Kernel::getSize()
{
	return size;
}

float** Kernel::getWeights()
{
	return weights;
}

Pixel Kernel::convolve(int x, int y, Image& pixels)
{
	PrecisePixel sum(0, 0, 0);

	for (int yOffset = -halfSize; yOffset <= halfSize; yOffset++) {

		int newY = y + yOffset;
		for (int xOffset = -halfSize; xOffset <= halfSize; xOffset++) {
			int newX = x + xOffset;
			sum += pixels.GetPixel(newX, newY) * weights[yOffset + halfSize][xOffset + halfSize];
		}
	}
	return sum.round();
}

Vec Kernel::gradient(Coord c, Image& img)
{

	const int halfSize = 1;

	PrecisePixel horizontal(0, 0, 0);
	PrecisePixel vertical(0, 0, 0);

	for (int yOffset = -halfSize; yOffset <= halfSize; yOffset++) {

		int newY = c.getY() + yOffset;
		for (int xOffset = -halfSize; xOffset <= halfSize; xOffset++) {
			int newX = c.getX() + xOffset;

			double hFactor = (double) xOffset * (halfSize + 1 - std::abs(yOffset)) / (4);
			double vFactor = (double) yOffset * (halfSize + 1 - std::abs(xOffset)) / (4);

			horizontal += img.GetPixel(newX, newY) * hFactor;
			vertical += img.GetPixel(newX, newY) * vFactor;

		}
	}

	double hSum = horizontal.getAvg() * 3;
	double vSum = vertical.getAvg() * 3;

	//std::cout << "Gradient direction is: " << std::atan(hSum / vSum) << std::endl;

	return Vec(vSum, hSum);
}

double* Kernel::variance(Coord c, ImageData& vecs, ImageData& mags)
{

	const int halfSize = 1;
	double vecVariance = 0;
	double magVariance = 0;

	double* result = new double[2];

	for (int yOffset = -halfSize; yOffset <= halfSize; yOffset++) {

		int newY = c.getY() + yOffset;
		for (int xOffset = -halfSize; xOffset <= halfSize; xOffset++) {
			int newX = c.getX() + xOffset;
			vecVariance += std::abs(vecs.GetData(newX, newY) - vecs.GetData(c.getX(), c.getY()));
			magVariance += std::abs(mags.GetData(newX, newY) - mags.GetData(c.getX(), c.getY()));

		}
	}
	result[0] = vecVariance;
	result[1] = magVariance;

	return result;
}
