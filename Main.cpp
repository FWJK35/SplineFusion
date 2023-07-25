#include "framework.h"

#include "Kernel.h"
#include "Line.h"

int main()
{
	using namespace std;

	ifstream image;
	ofstream newImage;
	string filename = "blank.ppm";
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

	

	cout << "Convolving edge detection";
	Kernel edge(3, "edge");
	//ogPixels.ConvolveTo(newPixels, edge, 10);
	//newPixels.CopyTo(ogPixels);
	cout << endl << "Edge detection complete." << endl;

	cout << "Blurring edges";
	Kernel blur(3, "focusblur");
	//ogPixels.ConvolveTo(newPixels, blur, 10);
	//newPixels.CopyTo(ogPixels);
	cout << endl << "Blurring edges complete." << endl;

	cout << "Grayscaling";
	//ogPixels.GrayScale();
	//ogPixels.CopyTo(newPixels);
	cout << endl << "Grayscaling complete." << endl;

	cout << "Generating lines";
	for (int i = 100; i <= 300; i += 20) {
		Line l(100, 100, 500, i);
		l.DrawTo(newPixels);
	}
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