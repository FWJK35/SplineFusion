#include "framework.h"

#include "Kernel.h"
#include "Line.h"
#include "CircleArc.h"
#include "Spline.h";
#include "ImageData.h"

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
	Kernel blur(3, "gaussblur");
	//ogPixels.ConvolveTo(newPixels, blur, 10);
	newPixels.CopyTo(ogPixels);
	cout << endl << "Blurring edges complete." << endl;


	cout << "Calculating Gradients";
	Kernel g(3);
	ImageData slopeVectors(width, height);
	ImageData slopeMagnitudes(width, height);

	//calculate initial edge info (magnitude and direction)
	for (int y = 0; y < height; y++) {
		if (y % (height / 10) == 0) {
			cout << ".";
		}
		for (int x = 0; x < width; x++) {
			Vec grad = Kernel::gradient(Coord(x, y), ogPixels);
			if (grad.getX() * grad.getY() >= 0) {
				slopeVectors.WriteData(x, y, atan(grad.getY() / grad.getX()));
			}
			else {
				slopeVectors.WriteData(x, y, atan(grad.getY() / grad.getX()) + M_PI);
			}
			
			if (grad.getX() == 0) slopeVectors.WriteData(x, y, 0);
			slopeMagnitudes.WriteData(x, y, grad.getDistance(Vec(0, 0)) / 255 / 3);
		}
	}
	cout << endl;
	double max = 0;
	for (int y = 0; y < height; y++) {
		if (y % (height / 10) == 0) {
			cout << ".";
		}
		for (int x = 0; x < width; x++) {
			double hue = slopeVectors.GetData(x, y) * M_1_PI * 360;
			double sat = 1;

			double threshold = 0.02;
			double val = sqrt(slopeMagnitudes.GetData(x, y) / (1 - threshold) - threshold);


			double* var = Kernel::variance(Coord(x, y), slopeVectors, slopeMagnitudes);
			//if (var[1] > 0.5) sat = 0;
			//double val = sqrt(slopeMagnitudes.GetData(x, y));
			PrecisePixel gradPix = PrecisePixel::fromHSV(hue, sat, val);
			newPixels.WriteData(x, y, gradPix.round());
		}
	}
	newPixels.CopyTo(ogPixels);

	cout << "Convolving edge detection";

	//Kernel h(3, "sobelh");
	//Image hPixels(width, height);
	////ogPixels.ConvolveTo(hPixels, h, 10);

	//Kernel v(3, "sobelv");
	//Image vPixels(width, height);
	////ogPixels.ConvolveTo(vPixels, v, 10);


	cout << endl << "Edge detection complete." << endl;

	cout << "Blurring edges";
	Kernel focusblur(3, "focusblur");
	//ogPixels.ConvolveTo(newPixels, focusblur, 10);
	//newPixels.CopyTo(ogPixels);
	//ogPixels.ConvolveTo(newPixels, blur, 10);
	cout << endl << "Blurring edges complete." << endl;

	cout << "Grayscaling";
	//ogPixels.GrayScale();
	//ogPixels.CopyTo(newPixels);
	cout << endl << "Grayscaling complete." << endl;

	cout << "Generating lines";
	Line l(Vec(100, 100), Vec(200, 150));
	//l.DrawTo(newPixels);
	CircleArc c(Vec(300, 100), Vec(400, 200), 4.5);
	//c.DrawTo(newPixels);
	Spline s1(Vec(100, 300), Vec(200, 100), Vec(300, 400), Vec(300, 200));
	//s1.drawTo(newPixels);

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