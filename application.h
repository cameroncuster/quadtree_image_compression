#ifndef APPLICATION_H
#define APPLICATION_H
#include "olcPixelGameEngine.h"
#include "readPNG.h"

typedef unsigned char byte;

class Application : public olc::PixelGameEngine
{
   const char *filename;
   olc::Pixel **image;
   byte **greyScale;
   unsigned width, height;
   int threshold;

public:
   Application(const char *, int);

public:
   bool OnUserCreate() override;
   bool OnUserUpdate(float) override;
   bool OnUserDestroy() override;

   int Width() const;
   int Height() const;
};

#endif
