#include "framework.h"

#include "Canny.h"
#include "Kernel.h"
#include "Line.h"
#include "CircleArc.h"
#include "Spline.h"
#include "EdgeGroup.h"

int main()
{
	using namespace std;

	ifstream image;
	ofstream newImage;
	string filename = "firefly.ppm";
	image.open(filename, std::ios::binary);
	newImage.open("new" + filename, std::ios::binary);

	char currentChar = 0;
	//type header
	while (currentChar != '\n') {
		image.get(currentChar);
		newImage.put(currentChar);
	}

	string widthStr, heightStr;
	int width, height;
	//size width header
	while (currentChar != ' ') {
		image.get(currentChar);
		newImage.put(currentChar);
		widthStr += currentChar;
	}
	width = stoi(widthStr);

	//size height header
	while (currentChar != '\n') {
		image.get(currentChar);
		newImage.put(currentChar);
		heightStr += currentChar;
	}
	height = stoi(heightStr);

	//rgb header
	do {
		image.get(currentChar);
		newImage.put(currentChar);
	} while (currentChar != '\n');

	//2D vector containing all pixels in the image
	Image ogPixels(width, height);
	Image newPixels(width, height);

	//copy image data from PPM file to RAM
	cout << "Copying image data";
	for (int y = 0; y < height; y++) {
		if (y % (height / 10) == 0) {
			cout << ".";
		}
		for (int x = 0; x < width; x++) {
			char pix[3] = { 0, 0, 0 };
			image.read(pix, 3);
			uint8_t r = pix[0];
			uint8_t g = pix[1];
			uint8_t b = pix[2];
			Pixel thisPixel = { r, g, b };

			ogPixels.WriteData(x, y, thisPixel);
		}
	}
	ogPixels.CopyTo(newPixels);
	cout << endl << "Image data copying complete." << endl;

	//initial blur
	cout << "Blurring edges";
	Kernel blur(3, "gauss", 1);
	Image blurred(ogPixels);
	ogPixels.ConvolveTo(blurred, blur, 10);
	blurred.CopyTo(ogPixels);
	cout << endl << "Blurring edges complete." << endl;

	cout << "Calculating Sobel Gradients" << endl;

	VecField edgeVecs(width, height);
	ImageData slopeNormalDirections(width, height);
	ImageData slopeDirections(width, height);
	ImageData slopeMagnitudes(width, height);

	Canny::Sobel(ogPixels, slopeNormalDirections, slopeMagnitudes, edgeVecs);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			double normal = slopeNormalDirections.GetData(x, y);
			if (normal > M_PI_2)
				slopeDirections.WriteData(x, y, normal - M_PI_2);
			else
				slopeDirections.WriteData(x, y, normal + M_PI_2);
		}
	}

	cout << "Calculating Laplacian" << endl;

	ImageData laplacian(width, height);

	Canny::Laplacian(ogPixels, slopeNormalDirections, slopeMagnitudes, laplacian);

	cout << "Thresholding Edges" << endl;

	ImageData thresholded(width, height);

	Canny::Threshold(laplacian, slopeMagnitudes, 0.01, 0.05, thresholded);
	//ImageData thresholded contains all pixels that are part of an edge

	cout << "Finding Edge Groups" << endl;
	vector<EdgeGroup*> edgeGroups;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (thresholded.GetData(x, y) == 1) {

				EdgeGroup* group = new EdgeGroup(x, y, thresholded, slopeDirections);
				edgeGroups.push_back(group);
				while ((*group).GetSplitLocation() >= 0) {
					if (!(*group).getCyclic()) {
						edgeGroups.push_back((*group).Split((*group).GetSplitLocation()));
					}
					else
					{
						(*group).Split((*group).GetSplitLocation());
					}
				}
			}
		}
	}
	edgeGroups.push_back((*edgeGroups[0]).Split(10));
	//color pixels by edgegroup data
	for (EdgeGroup* g : edgeGroups) {
		EdgeGroup group = *g;
		//double color = (std::rand() % 360) / 360.0 * M_PI;
		//double color = M_PI * (edgeGroups.end() - g) / edgeGroups.size();
		double color = (group.getSize() % 360) / 360.0 * M_PI;
		//double sat = 1 - group.getVariation() * M_2_PI;
		int index = 0;
		for (int i = 0; i < group.getSize(); i++) {

			//(*n).printData();
			Coord c = group.getFinalPoints()[i];

			thresholded.WriteData(c, 1);
			slopeMagnitudes.WriteData(c, 1 - group.getSlopeDerivatives()[i] / M_PI_2);
			if (group.getSize() <= 2)
				thresholded.WriteData(c, 0);

			slopeDirections.WriteData(c, color);
			//slopeDirections.WriteData(c, M_PI * index / group.getFinalPoints().size());
			index ++;
		}
		
	}

	//converts data to pixel data
	double max = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			

			double hue = slopeDirections.GetData(x, y) * M_1_PI * 360;
			double sat = slopeMagnitudes.GetData(x, y);
			double val = thresholded.GetData(x, y);

			double threshold = 0.02;
			//double val = sqrt(slopeMagnitudes.GetData(x, y) / (1 - threshold) - threshold);


			//double* var = Kernel::variance(Coord(x, y), slopeDirections, slopeMagnitudes);
			//if (var[1] > 0.5) sat = 0;
			
			
			//val = sat;
			PrecisePixel gradPix = PrecisePixel::fromHSV(hue, sat, val);
			newPixels.WriteData(x, y, gradPix.round());
		}
	}

	cout << "Grayscaling";
	//ogPixels.GrayScale();
	//ogPixels.CopyTo(newPixels);
	cout << endl << "Grayscaling complete." << endl;

	cout << "Generating lines";

	cout << endl << "Line generation complete." << endl;

	cout << "Writing new data";
	for (int y = 0; y < height; y++) {
		if (y % (height / 10) == 0) {
			cout << ".";
		}
		for (int x = 0; x < width; x++) {
			char* pix = newPixels.GetData(x, y);
			newImage << pix[0] << pix[1] << pix[2];
		}
	}
	cout << endl << "Image data writing complete.";

	image.close();
	newImage.close();

	return 0;
}