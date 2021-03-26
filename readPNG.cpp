#include "readPNG.h"

// readPNG - Read a PNG file into a two dimensional array of olc::Pixel objects
// filename[in] character string holding the file name to read
// width [out]  integer returning the width of the PNG image
// height [out] integer returning the height of the PNG image
// returns 2d array of olc::Pixel objects representing the PNG image
int **readPNG(const char *filename, unsigned &width, unsigned &height)
{
    unsigned char *pixels;
    // lodepng_decode_file does the heavy lifting in decoding the PNG image
    // file format.  It will return a pointer to the pixels that are read from
    // the file.
    unsigned error = lodepng_decode_file(&pixels, &width, &height, filename, LCT_RGBA, 8);

    if (error)
    {
        cout << "Decoder error while reading file " << filename << endl;
        cout << "error code " << error << " " << lodepng_error_text(error) << "\n";
        return nullptr;
    }

    cout << "Read file " << width << " x " << height << "\n";

    // The width of the image needs to be a whole multiple of 4 (4 bytes in
    // and integer) for alignment reasons.  This code calculates that padding
    unsigned nbytes = width * 4;
    {
        unsigned pad = 0 ;
        if (width % 4 != 0) pad = 4 - width % 4;
        width += pad;
    }

    // Get a block of contiguous memory so we can index it with [row][col]
    int **image = alloc2D(height, width);
    for (int row = 0 ; row < (int)height ; row++, pixels += nbytes)
        memcpy(image[row], pixels, nbytes);

    // clean up after ourselves
    pixels -= (nbytes * height);
    free(pixels);

    // return the 2d array of Pixels
    return image;
}

byte colorToGreyscale(int &p)
{
    byte red = (byte)(p);
    byte green = (byte)(p >> 8);
    byte blue = (byte)(p >> 16);
    return (red * 0.3 + green * 0.6 + blue * 0.1 + 0.5);
}

byte **convertToGreyscale(int **image, unsigned width, unsigned height)
{
    byte **bmp = alloc2D_byte(height, width);

    for (unsigned row = 0 ; row < height ; row++)
        for (unsigned col = 0 ; col < width ; col++)
            bmp[row][col] = colorToGreyscale(image[row][col]);

    return bmp;
}
