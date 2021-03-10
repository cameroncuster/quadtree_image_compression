#include "quadTree.h"
#include <cmath>

using namespace std;

QuadTree::QuadTree( unsigned char **gray, unsigned char tolerance, unsigned width, unsigned height )
{
	margin = tolerance;
	// construct node
}

QuadTree::~QuadTree( )
{
}

unsigned char **QuadTree::getImage( ) const
{
	return nullptr;
}

vector<pair<unsigned, unsigned>> QuadTree::getImageBorders( ) const
{
	return { { 0, 0 } };
}

unsigned QuadTree::subdivide( unsigned char **gray, node *quadrant )
{
	if( !needSubdivide( gray, quadrant ) )
		return quadrant->pixelValue;
	return quadrant->pixelValue;
}

bool QuadTree::needSubdivide( unsigned char **gray, node *quadrant ) const
{
	unsigned i, j;
	unsigned char mx = gray[ quadrant->topLeft.y ][ quadrant->topLeft.x ],
	mn = gray[ quadrant->topLeft.y ][ quadrant->topLeft.x ];
	for( i = quadrant->topLeft.y; i < quadrant->bottomRight.y; i++ )
	{
		for( j = quadrant->topLeft.x; j < quadrant->bottomRight.x; j++ )
		{
			if( gray[i][j] > mx )
				mx = gray[i][j];
			if( gray[i][j] < mn )
				mn = gray[i][j];
			if( mx - mn > margin )
				return 0;
		}
	}
	return 1;
}

unsigned char **QuadTree::buildImage( )
{
	return 0;
}
