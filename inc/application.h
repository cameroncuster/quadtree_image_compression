#ifndef APPLICATION_H
#define APPLICATION_H
#include "olcPixelGameEngine.h"
#include "readPNG.h"
#include "quadTree.h"

typedef unsigned char byte;

class Application : public olc::PixelGameEngine
{
   const char *filename;
   olc::Pixel **image;
   byte **greyScale;
   unsigned width, height;
   byte threshold;
   QuadTree *quadTree;
   bool lines;
public:
    Application(const char *, byte);
    ~Application( );

public:
    bool OnUserCreate() override;
    bool OnUserUpdate(float) override;
    bool OnUserDestroy() override;

    int Width() const;
    int Height() const;

private:
    void informationAboutQuadTreeUpdate( ostream &out ) const;
};
#endif
