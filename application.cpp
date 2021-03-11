#include "application.h"
#include "quadTree.h"
#include <iostream>

using namespace std;

olc::Pixel byteToGreyscalePixel(byte pixelByte)
{
	return olc::Pixel(pixelByte, pixelByte, pixelByte);
}

int Application::Width() const { return width ; }
int Application::Height() const { return height ; }

Application::Application(const char *filename, int thresh) : filename(filename), threshold(thresh)
{
	sAppName = "Application";
	// read an RGBA PNG image from a file.  Need to read it here so we have the width
	// and height correct for opening the window with olc
	image = readPNG(filename, width, height);
}

// Called once at the start, so create things here
bool Application::OnUserCreate()
{
	// convert RGBA Pixels into greyscale values.  0 = black ... 255 = white
	greyScale = convertToGreyscale(image, width, height);
	// instantiate the QuadTree
	QuadTree qt( greyScale, threshold, width, height );
	greyScale = qt.draw( 0 );
	return true;
}

// This member function is called repeatedly until the program exits.
bool Application::OnUserUpdate(float fElapsedTime)
{
	// This draws the image into the window.
	for (int y = 0; y < ScreenHeight(); y++)
		for (int x = 0; x < ScreenWidth() / 2; x++)
		{
			// output the original image on the left
			Draw(x, y, image[y][x]);
			// output the greyscale image on the right
			Draw(x + width, y, byteToGreyscalePixel(greyScale[y][x]));
		}

	// If the escape or 'q' key is pressed, exit
	return (!(GetKey(olc::Key::ESCAPE).bPressed) &&
			!(GetKey(olc::Key::Q).bPressed));
}

bool Application::OnUserDestroy()
{
	free2D(greyScale);
	free2D(image);
	return true;
}
