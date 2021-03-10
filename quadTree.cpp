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

void QuadTree::subdivide( unsigned char **gray, node *quadrant )
{
	if( !needSubdivide( gray, quadrant ) )
	{
		quadrant->pixelValue = evalSubdivision( gray, quadrant );
		return;
	}

	subdivide( gray, quadrant->nw );
	subdivide( gray, quadrant->sw );
	subdivide( gray, quadrant->ne );
	subdivide( gray, quadrant->se );

	quadrant->pixelValue = evalSubdivision( gray, quadrant ); // potentially the average values of the children...
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

unsigned QuadTree::evalSubdivision( unsigned char **gray, node *quadrant ) const
{
	unsigned i, j;
	unsigned sum = 0;
	for( i = quadrant->topLeft.y; i < quadrant->bottomRight.y; i++ )
		for( j = quadrant->topLeft.x; j < quadrant->bottomRight.x; j++ )
			sum += gray[i][j];
	return sum / ( ( quadrant->bottomRight.y - quadrant->topLeft.y ) *
			( quadrant->bottomRight.x - quadrant->topLeft.x ) );
}

unsigned char **QuadTree::buildImage( )
{
	return 0;
}
