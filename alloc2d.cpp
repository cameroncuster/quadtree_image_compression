#include "alloc2d.h"

// allocate a 2d array of bytes which is indexable via [row][col] notation
byte **alloc2D_byte(int nrows, int ncols)
{
   byte **bmp = new byte*[nrows];
   if (bmp == nullptr) return nullptr;

   byte *bits = new byte[nrows * ncols];
   if (bits == nullptr) { delete [] bmp ; return nullptr ; }

   memset(bits, 0, nrows * ncols * sizeof(byte));

   for (int row = 0 ; row < nrows ; ++row, bits += ncols)
      bmp[row] = bits;

   return bmp;
}

int** alloc2D( int nrows, int ncols )
{
    // allocate a row of pointers
    int** image = new int* [nrows];
    if ( image == NULL ) return NULL;

    // allocate array elements
    int* pixels = new int [ nrows * ncols ];
    if ( pixels == NULL ) { delete [] image; return NULL; }

    // zero out array elements
    memset( pixels, 0, nrows * ncols * sizeof (int) );

    // point the row pointers at each row of the 2-D array
    for ( int row = 0; row < nrows; ++row, pixels += ncols )
        image[ row ] = pixels;

    // and return...
    return image;
}

// deallocate 2-D array
void free2D( int** image )
{
    if ( image )
    {
        delete [] *image;
        delete [] image;
    }
}

// deallocate 2-D array
void free2D( byte **bmp )
{
    if ( bmp )
    {
        delete [] *bmp;
        delete [] bmp;
    }
}
