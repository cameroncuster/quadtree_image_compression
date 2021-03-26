#include "catch.hpp"
#include "quadTree.h"
#include "alloc2d.h"
#include <iostream>

using namespace std;

/*
TEST_CASE( "Throws exception for null image" )
{
	unsigned width = 0;
	unsigned height = 0;
	byte thresh = 20;
	byte **testImage = nullptr;
	CHECK_THROWS( QuadTree { testImage, width, height, thresh } );
}
*/

TEST_CASE( "Test constructor for single pixel image" )
{
	unsigned width = 1;
	unsigned height = 1;
	unsigned thresh = 20;
	byte **testImage = alloc2D_byte( height, width );
	QuadTree quadTree( testImage, width, height, thresh );
	free2D( testImage );
}

TEST_CASE( "Test Quadtree object leafCount function, expect 1" )
{
	unsigned width = 1;
	unsigned height = 1;
	unsigned thresh = 20;
	byte **testImage = alloc2D_byte( height, width );

	QuadTree quadTree( testImage, width, height, thresh );

	REQUIRE( quadTree.leafCount( ) == 1);
	free2D( testImage );
}

TEST_CASE( "Test Quadtree returns correct greyscale image for uniform image" )
{
	unsigned i, j;
	unsigned width = 4;
	unsigned height = 4;
	unsigned thresh = 20;
	byte **testImage = alloc2D_byte( height, width );

	for( i = 0; i < 4; i++ )
		for( j = 0; j < 4; j++ )
			testImage[i][j] = 128;

	QuadTree quadTree( testImage, width, height, thresh );

	byte **compressedImage = quadTree.getCompressedImage( );

	for( i = 0; i < 4; i++ )
		for( j = 0; j < 4; j++ )
			REQUIRE( compressedImage[i][j] == 128 );

	free2D( testImage );
	free2D( compressedImage );
}

// 10 10  0  0
// 10 10  0  0
//  0  0 10 10
//  0  0 10 10
// You can expand on this idea.  Send an image with known characteristics and
// make sure the resulting compressed image is what you expect.  This
// test should probably be broken up into smaller unit tests
TEST_CASE( "Test Quadtree returns correct greyscale image for checkerboard image" )
{
	unsigned i, j;
	unsigned width = 4;
	unsigned height = 4;
	unsigned thresh = 5;
	byte **tImage = alloc2D_byte(height, width);

	tImage[0][0] = tImage[0][1] = 10; tImage[0][2] = tImage[0][3] =  0;
	tImage[1][0] = tImage[1][1] = 10; tImage[1][2] = tImage[1][3] =  0;
	tImage[2][0] = tImage[2][1] =  0; tImage[2][2] = tImage[2][3] = 10;
	tImage[3][0] = tImage[3][1] =  0; tImage[3][2] = tImage[3][3] = 10;

	QuadTree quadTree( tImage, width, height, thresh );

	byte **compressedImage = quadTree.getCompressedImage( );

	for( i = 0; i < 4; i++ )
		for( j = 0; j < 4; j++ )
			REQUIRE( compressedImage[i][j] == 5 );

	REQUIRE(quadTree.leafCount() == 1);

	// Decreasing the threshold (increasing image quality) should increase
	// node count
	quadTree.decrementThreshold( tImage );
	//REQUIRE( quadTree.leafCount( ) == 4 );

	compressedImage = quadTree.getCompressedImage( );
	for( i = 0; i < 4; i++ )
		for( j = 0; j < 4; j++ )
		{
			if( i < 2 && j < 2 )
				REQUIRE( compressedImage[i][j] == 10 );
			if( i < 2 && j > 1 )
				REQUIRE( compressedImage[i][j] == 0 );
			if( i > 1 && j < 2 )
				REQUIRE( compressedImage[i][j] == 0 );
			if( i > 1 && j > 1 )
				REQUIRE( compressedImage[i][j] == 10 );
		}

	free2D( tImage );
	free2D( compressedImage );
}
