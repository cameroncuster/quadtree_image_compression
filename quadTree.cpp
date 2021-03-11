#include "quadTree.h"
#include "alloc2d.h"
#include <cmath>

using namespace std;

QuadTree::QuadTree( unsigned char **gray, const unsigned char tolerance, const unsigned width, const unsigned height )
{
	pair<unsigned, unsigned> tl = { 0, 0 };
	pair<unsigned, unsigned> br = { width - 1, height - 1 };
	pixelCount = width * height;
	nodeCount = 0;
	leafNodeCount = 0;
	byteCount = 0; // malloc but don't free and ask Valgrind how much I used...
	compression = 0;
	margin = tolerance;
	root = new node( tl, br, evalSubdivision( gray, tl, br ) );
	subdivide( gray, root );
}

unsigned char **QuadTree::draw( const bool lines ) const
{
	unsigned char **gray = alloc2D_byte( root->bottomRight.first, root->bottomRight.second );
	// build the image from the tree
	return gray; // the image memory must be freed
}

void QuadTree::subdivide( unsigned char **&gray, node *quadrant )
{
	pair<unsigned, unsigned> nwtl = quadrant->topLeft;
	pair<unsigned, unsigned> nwbr = { quadrant->bottomRight.first / 2, quadrant->bottomRight.second / 2 };
	pair<unsigned, unsigned> swtl = { quadrant->topLeft.first, quadrant->topLeft.second / 2 };
	pair<unsigned, unsigned> swbr = { quadrant->bottomRight.first / 2, quadrant->bottomRight.second };
	pair<unsigned, unsigned> netl = { quadrant->topLeft.first / 2, quadrant->bottomRight.second };
	pair<unsigned, unsigned> nebr = { quadrant->bottomRight.first, quadrant->bottomRight.second / 2 };
	pair<unsigned, unsigned> setl = { quadrant->topLeft.first / 2, quadrant->topLeft.second / 2 };
	pair<unsigned, unsigned> sebr = quadrant->bottomRight;

	if( needSubdivide( gray, quadrant->pixelValue, nwtl, nwbr ) )
	{
		quadrant->nw = new node( nwtl, nwbr, evalSubdivision( gray, nwtl, nwbr ) );
		subdivide( gray, quadrant->nw );
		nodeCount++;
	}
	if( needSubdivide( gray, quadrant->pixelValue, swtl, swbr ) )
	{
		quadrant->sw = new node( swtl, swbr, evalSubdivision( gray, swtl, swbr ) );
		subdivide( gray, quadrant->sw );
		nodeCount++;
	}
	if( needSubdivide( gray, quadrant->pixelValue, netl, nebr ) )
	{
		quadrant->ne = new node( netl, nebr, evalSubdivision( gray, netl, nebr ) );
		subdivide( gray, quadrant->ne );
		nodeCount++;
	}
	if( needSubdivide( gray, quadrant->pixelValue, setl, sebr ) )
	{
		quadrant->se = new node( setl, sebr, evalSubdivision( gray, setl, sebr ) );
		subdivide( gray, quadrant->se );
		nodeCount++;
	}
}

bool QuadTree::needSubdivide( unsigned char **&gray, const unsigned char rep, const pair<unsigned, unsigned> topLeft, const pair<unsigned, unsigned> bottomRight ) const
{
	unsigned i, j;
	unsigned char mx = gray[ topLeft.second ][ topLeft.first ];
	unsigned char mn = gray[ topLeft.second ][ topLeft.first ];
	for( i = topLeft.second; i < bottomRight.second; i++ )
	{
		for( j = topLeft.first; j < bottomRight.first; j++ )
		{
			if( gray[i][j] > mx )
				mx = gray[i][j];
			if( gray[i][j] < mn )
				mn = gray[i][j];
			if( abs( rep - mx ) > margin )
				return 0;
			if( abs( rep - mn ) > margin )
				return 0;
		}
	}
	return 1;
}

unsigned QuadTree::evalSubdivision( unsigned char **&gray, const pair<unsigned, unsigned> topLeft, const pair<unsigned, unsigned> bottomRight ) const
{
	unsigned i, j;
	unsigned sum = 0;
	for( i = topLeft.second; i < bottomRight.second; i++ )
		for( j = topLeft.first; j < bottomRight.first; j++ )
			sum += gray[i][j];
	return sum / ( ( bottomRight.second - topLeft.second ) *
			( bottomRight.first - topLeft.first ) );
}

unsigned char **QuadTree::buildImage( )
{
	return 0;
}
