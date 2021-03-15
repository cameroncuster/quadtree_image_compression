#ifndef READPNG_H
#define READPNG_H
#include <iostream>
#include "lodepng.h"
#include "alloc2d.h"

using namespace std;

int **readPNG(const char *filename, unsigned &width, unsigned &height);
byte **convertToGreyscale(int **image, unsigned width, unsigned height);
#endif
