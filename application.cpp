#include "application.h"
#include <iostream>

using namespace std;

olc::Pixel byteToGreyscalePixel(byte pixelByte)
{
    return olc::Pixel(pixelByte, pixelByte, pixelByte);
}

int Application::Width() const { return width ; }
int Application::Height() const { return height ; }

// threshold instantiated with value [0 - 254] because a threshold outside the
// range [0 - 254] is not applicable for the application since this is the
// limitation of a pixel value ( e.a. a threshold of 255 is equivalent to any
// threshold greater than 255 in implementation )
Application::Application(const char *filename, byte thresh) : filename(filename), threshold(thresh), lines(0)
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

// formats information about the updated/new QuadTree and image compression for output log
void Application::informationAboutQuadTreeUpdate( ostream &out ) const
{
    out << "Leaves = " << quadTree->leafCount( ) << " mem: " <<
        2 * quadTree->leafCount( ) << " bytes: " << width * height <<
        " compressed size: " <<
        int( 100.0 * ( ( 2.0 * ( double )quadTree->leafCount( ) )
                    / ( double )( width * height ) )  + 0.5 ) << '%' <<
        " : Quality Factor " << '[' << ( unsigned short )threshold << ']' << endl;
}

// Called once at the start, so create things here
bool Application::OnUserCreate()
{
    // convert RGBA Pixels into greyscale values.  0 = black ... 255 = white
    greyScale = convertToGreyscale((int **)image, width, height);
    quadTree = new QuadTree( greyScale, width, height, threshold % 255 );
    informationAboutQuadTreeUpdate( cout );
    return true;
}

// This member function is called repeatedly until the program exits.
bool Application::OnUserUpdate(float fElapsedTime)
{
    if( GetKey( olc::Key::SPACE ).bPressed )
        lines = !lines;
    if( GetKey( olc::Key::UP ).bPressed )
    {
        quadTree->incrementThreshold( greyScale );
        if( threshold != quadTree->threshold( ) )
            informationAboutQuadTreeUpdate( cout );
    }
    if( GetKey( olc::Key::DOWN ).bPressed )
    {
        quadTree->decrementThreshold( greyScale );
        if( threshold != quadTree->threshold( ) )
            informationAboutQuadTreeUpdate( cout );
    }

    threshold = quadTree->threshold( );

    // set the compressed image to the quadTree compressed image
    compressed = quadTree->getCompressedImage( );

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
