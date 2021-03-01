#ifndef READPNG_H
#define READPNG_H
#include <iostream>
#include "lodepng.h"
#include "olcPixelGameEngine.h"
#include "alloc2d.h"

using namespace std;

olc::Pixel **readPNG(const char *filename, unsigned &width, unsigned &height);
byte **convertToGreyscale(olc::Pixel **image, unsigned width, unsigned height);
#endif
