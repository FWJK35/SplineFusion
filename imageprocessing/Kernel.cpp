#include "Kernel.h"

Kernel::Kernel(int aSize, std::string aType, double stdv)
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
			else if (type == "gauss") {
				double distx = (j - halfSize);
				double disty = (i - halfSize);
				double calc = exp((distx * distx + disty * disty) /  (-2 * (stdv * stdv)));
				weights[i][j] = calc;
			}
			else if (type == "laplace") {
				double distx = (j - halfSize);
				double disty = (i - halfSize);
				double calc = ((distx * distx + disty * disty) / (2 * stdv * stdv) - 1) * exp((distx * distx + disty * disty) / (-2 * (stdv * stdv)));
				
				weights[i][j] = calc;
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
	if (type == "gauss" || type == "laplace") {
		double sum = 0;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				sum += weights[i][j];
			}
		}
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				weights[i][j] /= sum;
				std::cout << weights[i][j] << " ";
				if (j == size - 1) {
					std::cout << std::endl;
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

void Kernel::setWeight(int x, int y, float newWeight)
{
	weights[y][x] = newWeight;
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

double Kernel::convolve(int x, int y, ImageData& imgData)
{
	double sum = 0;

	for (int yOffset = -halfSize; yOffset <= halfSize; yOffset++) {

		int newY = y + yOffset;
		for (int xOffset = -halfSize; xOffset <= halfSize; xOffset++) {
			int newX = x + xOffset;
			sum += imgData.GetData(newX, newY) * weights[yOffset + halfSize][xOffset + halfSize];
		}
	}

	return sum;
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
