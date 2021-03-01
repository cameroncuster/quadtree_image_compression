#ifndef ALLOC2D_H
#define ALLOC2D_H

#include <cstring>
#include "olcPixelGameEngine.h"

typedef unsigned char byte;

byte **alloc2D_byte( int nrows, int ncols );
olc::Pixel** alloc2D( int nrows, int ncols );
void free2D( byte **image );
void free2D( olc::Pixel** image );

#endif
