#include "Canny.h"

/*
* Performs Sobel edge detection on each pixel, outputting the
* direction and magnitudes to an array of directions, an array of magnitudes,
* and an array of vectors
*/
void Canny::Sobel(Image& img, ImageData& directions, ImageData& magnitudes, VecField& output)
{
	for (int y = 0; y < img.getHeight(); y++) {
		for (int x = 0; x < img.getWidth(); x++) {

			PrecisePixel xComp(0, 0, 0);
			PrecisePixel yComp(0, 0, 0);

			for (int yOffset = -1; yOffset <= 1; yOffset++) {

				int newY = y + yOffset;
				for (int xOffset = -1; xOffset <= 1; xOffset++) {
					int newX = x + xOffset;

					double xFactor = (double)xOffset * (2 - std::abs(yOffset)) / 4;
					double yFactor = (double)yOffset * (2 - std::abs(xOffset)) / -4;

					xComp += img.GetPixel(newX, newY) * xFactor;
					yComp += img.GetPixel(newX, newY) * yFactor;

				}
			}

			Coord c(x, y);
			double xCompAvg = xComp.getAvg() / 255;
			double yCompAvg = yComp.getAvg() / 255;

			Vec gradient(xCompAvg, yCompAvg);
			output.WriteData(c, gradient);

			double direction;
			
			if (gradient.getX() * gradient.getY() >= 0) {
				direction = atan(yCompAvg / xCompAvg);
			}
			else {
				direction = atan(yCompAvg / xCompAvg) + M_PI;
			}
			//account for divide by 0 errors
			if (xCompAvg == 0) if (yCompAvg == 0) direction = 0; else direction = M_PI_2;

			double magnitude = std::sqrt(xCompAvg * xCompAvg + yCompAvg * yCompAvg) * M_SQRT1_2;

			directions.WriteData(c, direction);
			magnitudes.WriteData(c, magnitude);

		}
	}
}

/*
* Performs a Laplacian transformation on the sobel filtered image,
* isolating the local maxima in the direction normal to the edge,
* outputting the local maximum pixels to an output array as a 1 or 0
*/
void Canny::Laplacian(Image& img, ImageData& directions, ImageData& magnitudes, ImageData& output)
{
	for (int y = 0; y < img.getHeight(); y++) {
		for (int x = 0; x < img.getWidth(); x++) {
			Coord c(x, y);
			
			double newMag = magnitudes.GetData(x, y);
			double direction = directions.GetData(x, y);
			int quarter = (int)(direction * M_1_PI * 4);

			//this line changes which pixels are marked as edges in laplacian
			direction = quarter * M_PI_4;

			double firstInterpolation = quarter + 1 - direction * M_1_PI * 4;
			double secondInterpolation = direction * M_1_PI * 4 - quarter;

			double rightResult = 0;
			double leftResult = 0;
			
			PrecisePixel centerPixel = img.GetPixel(x, y) * -2.0;
			PrecisePixel rightPixel(0, 0, 0);
			PrecisePixel leftPixel(0, 0, 0);

			switch (quarter) {
			case 0:
				rightResult += magnitudes.GetData(x + 1, y) * firstInterpolation;
				rightResult += magnitudes.GetData(x + 1, y - 1) * secondInterpolation;
				leftResult += magnitudes.GetData(x - 1, y) * firstInterpolation;
				leftResult += magnitudes.GetData(x - 1, y + 1) * secondInterpolation;
				break;
			case 1:
				rightResult += magnitudes.GetData(x + 1, y - 1) * firstInterpolation;
				rightResult += magnitudes.GetData(x, y + 1) * secondInterpolation;
				leftResult += magnitudes.GetData(x - 1, y + 1) * firstInterpolation;
				leftResult += magnitudes.GetData(x, y - 1) * secondInterpolation;
				break;
			case 2:
				rightResult += magnitudes.GetData(x, y + 1) * firstInterpolation;
				rightResult += magnitudes.GetData(x - 1, y - 1) * secondInterpolation;
				leftResult += magnitudes.GetData(x, y - 1) * firstInterpolation;
				leftResult += magnitudes.GetData(x + 1, y + 1) * secondInterpolation;
				break;
			case 3:
				rightResult += magnitudes.GetData(x - 1, y - 1) * firstInterpolation;
				rightResult += magnitudes.GetData(x - 1, y) * secondInterpolation;
				leftResult += magnitudes.GetData(x + 1, y + 1) * firstInterpolation;
				leftResult += magnitudes.GetData(x + 1, y) * secondInterpolation;
				break;
			}

			if (newMag > leftResult && newMag > rightResult) {
				output.WriteData(c, 1);
			}
			else {
				output.WriteData(c, 0);
			}



		}
	}
}

/*
* Uses a threshold filter to keep only significant local maxima,
* and if a pixel is questionably significant, it will see if other 
* significant values point towards it.
*/
void Canny::Threshold(ImageData& edgePixels, ImageData& magnitudes, double lower, double upper, ImageData& output)
{
	//initial thresholding
	for (int y = 0; y < edgePixels.getHeight(); y++) {
		for (int x = 0; x < edgePixels.getWidth(); x++) {
			//only bother thresholding if it is a possible edge
			if (edgePixels.GetData(x, y) == 1) {

				if (magnitudes.GetData(x, y) < lower) {
					output.WriteData(x, y, 0);
				}
				else if (magnitudes.GetData(x, y) > upper) {
					output.WriteData(x, y, 1);
				}
				else {
					output.WriteData(x, y, 0.5);
				}
			}
		}
	}

	//connect all "strong" pixels to "questionable" ones
	for (int y = 0; y < edgePixels.getHeight(); y++) {
		for (int x = 0; x < edgePixels.getWidth(); x++) {
			if (output.GetData(x, y) == 1) {
				ConnectThreshold(output, x, y);
			}
		}
	}
	//erase all remaining questionable pixels as faded pixels
	for (int y = 0; y < edgePixels.getHeight(); y++) {
		for (int x = 0; x < edgePixels.getWidth(); x++) {
			if (output.GetData(x, y) == 0.5) {
				output.WriteData(x, y, 0.1);
			}
		}
	}
}

/*
* Connects all "questionable" adjacent pixels and writes them as "strong,
* then recursively calls itself on the new "strong" pixel
*/
void Canny::ConnectThreshold(ImageData& input, int x, int y)
{
	for (int yOffset = -1; yOffset <= 1; yOffset++) {
		int newY = y + yOffset;
		for (int xOffset = -1; xOffset <= 1; xOffset++) {
			int newX = x + xOffset;
			if (input.GetData(newX, newY) == 0.5) {
				input.WriteData(newX, newY, 1);
				ConnectThreshold(input, newX, newY);
			}
		}
	}
}
