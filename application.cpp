#include "application.h"
#include <iostream>

using namespace std;

olc::Pixel byteToGreyscalePixel(byte pixelByte)
{
	return olc::Pixel(pixelByte, pixelByte, pixelByte);
}

int Application::Width() const { return width ; }
int Application::Height() const { return height ; }

Application::Application(const char *filename, int thresh) : filename(filename), threshold(thresh), lines(0)
{
	sAppName = "Application";
	// read an RGBA PNG image from a file.  Need to read it here so we have the width
	// and height correct for opening the window with olc
	image = (olc::Pixel **)readPNG(filename, width, height);
}

Application::~Application( )
{
	delete quadTree;
}

// Outputs information about the QuadTree and image compression
void Application::output( ) const
{
	cout << "Leaves = " << quadTree->leafCount( ) << " mem: " << width * height <<
		" bytes: " << 2 * quadTree->leafCount( ) << " compressed size: " <<
		int( 100.0 * ( ( 2.0 * ( double )quadTree->leafCount( ) )
					/ ( double )( width * height ) )  + 0.5 ) << '%' <<
		" : Quality Factor " << '[' << threshold << ']' << endl;
}

// Called once at the start, so create things here
bool Application::OnUserCreate()
{
	// convert RGBA Pixels into greyscale values.  0 = black ... 255 = white
	greyScale = convertToGreyscale((int **)image, width, height);
	compressed = convertToGreyscale((int **)image, width, height);
	quadTree = new QuadTree( greyScale, width, height, threshold );
	output( );
	return true;
}

// This member function is called repeatedly until the program exits.
bool Application::OnUserUpdate(float fElapsedTime)
{
	if( GetKey( olc::Key::SPACE ).bPressed )
		lines = !lines;
	if( GetKey( olc::Key::UP ).bPressed )
	{
		quadTree->increaseThreshold( greyScale, width, height );
		if( threshold != quadTree->getThreshold( ) )
			output( );
	}
	if( GetKey( olc::Key::DOWN ).bPressed )
	{
		quadTree->decreaseThreshold( greyScale, width, height );
		if( threshold != quadTree->getThreshold( ) )
			output( );
	}

	threshold = quadTree->getThreshold( );

	// set the compressed image to the quadTree compressed image
	quadTree->getCompressedImage( compressed );

	// draw the lines if the lines are flagged true
	if( lines )
		quadTree->drawLines( compressed );

	// This draws the QR image into the window.
	for (int y = 0; y < ScreenHeight(); y++)
		for (int x = 0; x < ScreenWidth() / 2; x++)
		{
			// output the original image on the left
			Draw(x, y, image[y][x]);
			// output the compressed greyscale image on the right
			Draw(x + width, y, byteToGreyscalePixel(compressed[y][x]));
		}

	// If the escape or 'q' key is pressed, exit
	return (!(GetKey(olc::Key::ESCAPE).bPressed) &&
			!(GetKey(olc::Key::Q).bPressed));
}

bool Application::OnUserDestroy()
{
	free2D(greyScale);
	free2D((int **)image);
	return true;
}
