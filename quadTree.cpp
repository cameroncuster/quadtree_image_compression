#include "quadTree.h"
#include <cmath>

using namespace std;

QuadTree::QuadTree( const unsigned char **gray, const unsigned char tolerance, const unsigned width, const unsigned height )
{
	margin = tolerance;
	pair<unsigned, unsigned> tl = { 0, 0 };
	pair<unsigned, unsigned> br = { width - 1, height - 1 };
	root = new node( tl, br, evalSubdivision( gray, tl, br ) );
	subdivide( gray, root );
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

void QuadTree::subdivide( const unsigned char **&gray, node *quadrant )
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
	}
	if( needSubdivide( gray, quadrant->pixelValue, swtl, swbr ) )
	{
		quadrant->sw = new node( swtl, swbr, evalSubdivision( gray, swtl, swbr ) );
		subdivide( gray, quadrant->sw );
	}
	if( needSubdivide( gray, quadrant->pixelValue, netl, nebr ) )
	{
		quadrant->ne = new node( netl, nebr, evalSubdivision( gray, netl, nebr ) );
		subdivide( gray, quadrant->ne );
	}
	if( needSubdivide( gray, quadrant->pixelValue, setl, sebr ) )
	{
		quadrant->se = new node( setl, sebr, evalSubdivision( gray, setl, sebr ) );
		subdivide( gray, quadrant->se );
	}
}

bool QuadTree::needSubdivide( const unsigned char **&gray, const unsigned char rep, const pair<unsigned, unsigned> topLeft, const pair<unsigned, unsigned> bottomRight ) const
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

unsigned QuadTree::evalSubdivision( const unsigned char **&gray, const pair<unsigned, unsigned> topLeft, const pair<unsigned, unsigned> bottomRight ) const
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
