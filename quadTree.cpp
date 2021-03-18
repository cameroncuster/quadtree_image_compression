#include "quadTree.h"
#include "alloc2d.h"
#include <cmath>
#include <iostream>

using namespace std;

QuadTree::QuadTree( byte **&gray, const unsigned width, const unsigned height, const byte thresh ) :
	threshold( thresh ), pixelCount( width * height ), nodeCount( 0 )
{
	if( gray == nullptr || !width || !height )
		throw;
	pair<unsigned, unsigned> tl = { 0, 0 };
	pair<unsigned, unsigned> br = { width, height };
	leafNodeCount = 0;
	byteCount = 0; // malloc but don't free and ask Valgrind how much I used...
	compression = 0;
	root = subdivide( gray, tl, br );
}

QuadTree::~QuadTree( )
{
	clear( root );
	delete root;
}

void QuadTree::decreaseThreshold( byte **&gray, const unsigned width, const unsigned height ) // optomized for rapid protoyping must be an delete routine
{
	pair<unsigned, unsigned> tl = { 0, 0 };
	pair<unsigned, unsigned> br = { width, height };
	if( threshold == 255 )
		return;
	threshold++;
	clear( root );
	root = subdivide( gray, tl, br );
}

void QuadTree::increaseThreshold( byte **&gray, const unsigned width, const unsigned height ) // optomized for rapid protoyping must be an delete routine
{
	pair<unsigned, unsigned> tl = { 0, 0 };
	pair<unsigned, unsigned> br = { width, height };
	if( !threshold )
		return;
	threshold--;
	clear( root );
	root = subdivide( gray, tl, br );
}

void QuadTree::drawLines( byte **&gray ) const
{
	addLines( gray, root );
}

void QuadTree::addLines( byte **&gray, const node *quadrant ) const // rapid protoyping: lines could be stored externally from the quadTree node structure
{
	unsigned i;
	if( quadrant->nw == nullptr && quadrant->sw == nullptr &&
			quadrant->ne == nullptr && quadrant->se == nullptr ) // less operations than overwriting every recursive call
		return;

	for( i = quadrant->topLeft.second; i < quadrant->bottomRight.second; i++ )
	{
		gray[i][ quadrant->topLeft.first ] = 255;
		gray[i][ quadrant->bottomRight.first - 1 ] = 255;
	}

	for( i = quadrant->topLeft.first; i < quadrant->bottomRight.first; i++ )
	{
		gray[ quadrant->topLeft.second ][i] = 255;
		gray[ quadrant->bottomRight.second - 1 ][i] = 255;
	}

	addLines( gray, quadrant->nw );
	addLines( gray, quadrant->ne );
	addLines( gray, quadrant->sw );
	addLines( gray, quadrant->se );
}

unsigned QuadTree::leafCount( ) const
{
	return leafNodeCount;
}

void QuadTree::getCompressedImage( byte **&gray ) const
{
	buildCompressedImage( gray, root );
}

void QuadTree::buildCompressedImage( byte **&gray, const node *quadrant ) const
{
	unsigned i, j;
	if( quadrant->nw == nullptr && quadrant->sw == nullptr &&
			quadrant->ne == nullptr && quadrant->se == nullptr ) // less operations than overwriting every recursive call
	{
		for( i = quadrant->topLeft.second; i < quadrant->bottomRight.second; i++ )
			for( j = quadrant->topLeft.first; j < quadrant->bottomRight.first; j++ )
				gray[i][j] = quadrant->pixelValue;
		return;
	}

	buildCompressedImage( gray, quadrant->nw );
	buildCompressedImage( gray, quadrant->ne );
	buildCompressedImage( gray, quadrant->sw );
	buildCompressedImage( gray, quadrant->se );
}

QuadTree::node *QuadTree::subdivide( byte **&gray, pair<unsigned, unsigned> topLeft, pair<unsigned, unsigned> bottomRight )
{
	node *quadrant = new node( topLeft, bottomRight, evalSubdivision( gray, topLeft, bottomRight ) );
	pair<unsigned, unsigned> center = { topLeft.first + ( bottomRight.first - topLeft.first ) / 2, topLeft.second + ( bottomRight.second - topLeft.second ) / 2 };
	pair<unsigned, unsigned> nwtl = topLeft;
	pair<unsigned, unsigned> nwbr = center;
	pair<unsigned, unsigned> swtl = { topLeft.first, center.second };
	pair<unsigned, unsigned> swbr = { center.first, bottomRight.second };
	pair<unsigned, unsigned> netl = { center.first, topLeft.second };
	pair<unsigned, unsigned> nebr = { bottomRight.first, center.second };
	pair<unsigned, unsigned> setl = center;
	pair<unsigned, unsigned> sebr = bottomRight;

	if( !needSubdivide( gray, evalSubdivision( gray, topLeft, bottomRight ), topLeft, bottomRight ) ||
			topLeft.first - bottomRight.first < 2 || topLeft.second - bottomRight.second < 2 )
		return quadrant;

	quadrant->nw = subdivide( gray, nwtl, nwbr );
	quadrant->sw = subdivide( gray, swtl, swbr );
	quadrant->ne = subdivide( gray, netl, nebr );
	quadrant->se = subdivide( gray, setl, sebr );
	return quadrant;
}

bool QuadTree::needSubdivide( byte **&gray, const byte rep, const pair<unsigned, unsigned> topLeft, const pair<unsigned, unsigned> bottomRight ) const
{
	unsigned i, j;
	byte mx = gray[ topLeft.second ][ topLeft.first ];
	byte mn = gray[ topLeft.second ][ topLeft.first ];
	for( i = topLeft.second; i < bottomRight.second; i++ )
		for( j = topLeft.first; j < bottomRight.first; j++ )
		{
			if( gray[i][j] > mx )
				mx = gray[i][j];
			if( gray[i][j] < mn )
				mn = gray[i][j];
			if( mx - mn > threshold )
				return 1;
			/*
			if( abs( rep - mx ) > threshold )
				return 1;
			if( abs( rep - mn ) > threshold )
				return 1;
				*/
		}
	return 0;
}

unsigned QuadTree::evalSubdivision( byte **&gray, const pair<unsigned, unsigned> topLeft, const pair<unsigned, unsigned> bottomRight ) const
{
	unsigned i, j;
	unsigned sum = 0;
	if( bottomRight.second - topLeft.second == 0 || bottomRight.first - topLeft.first == 0 )
		return 0;
	for( i = topLeft.second; i < bottomRight.second; i++ )
		for( j = topLeft.first; j < bottomRight.first; j++ )
			sum += gray[i][j];
	return sum / ( ( bottomRight.second - topLeft.second ) *
			( bottomRight.first - topLeft.first ) );
}

void QuadTree::clear( node *n )
{
	if( n == nullptr )
	{
		delete n;
		return;
	}

	clear( n->nw );
	clear( n->sw );
	clear( n->ne );
	clear( n->se );
}
