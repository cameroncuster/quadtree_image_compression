#include "quadTree.h"
#include "alloc2d.h"
#include <cmath>
#include <iostream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
///			Construct a new QuadTree given an image and a threshold			 ///
////////////////////////////////////////////////////////////////////////////////
QuadTree::QuadTree( byte **&gray, const unsigned width, const unsigned height,
		const byte thresh ) :
	threshold( thresh ), pixelCount( width * height ), nodeCount( 0 )
{
	if( gray == nullptr || !width || !height )
		throw;
	pair<unsigned, unsigned> tl = { 0, 0 };
	pair<unsigned, unsigned> br = { width, height };
	leafNodeCount = 0;
	// 56 bytes per quadrantaccording to valgrind
	byteCount = 0; // malloc but don't free and ask Valgrind how much I used...
	compression = 0;
	uncompressedSize = width * height;
	root = subdivide( gray, tl, br );
	printData();
}

QuadTree::~QuadTree( )
{
	clear( root );
	delete root;
}

////////////////////////////////////////////////////////////////////////////////
///		   insert nodes as necessary to maintain the tighter threshold		 ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::decreaseThreshold( byte **&gray, const unsigned width,
		const unsigned height ) // optomized for rapid protoyping must be an delete routine
{
	leafNodeCount = 0;
	byteCount = 0;
	compression = 0;
	pair<unsigned, unsigned> tl = { 0, 0 };
	pair<unsigned, unsigned> br = { width, height };
	if( threshold == 255 )
		return;
	threshold++;
	clear( root );
	root = subdivide( gray, tl, br );
	printData();
}

////////////////////////////////////////////////////////////////////////////////
///		   delete nodes while possible to maintain the looser threshold		 ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::increaseThreshold( byte **&gray, const unsigned width,
		const unsigned height ) // optomized for rapid protoyping must be an delete routine
{
	leafNodeCount = 0;
	byteCount = 0;
	compression = 0;
	pair<unsigned, unsigned> tl = { 0, 0 };
	pair<unsigned, unsigned> br = { width, height };
	if( !threshold )
		return;
	threshold--;
	clear( root );
	root = subdivide( gray, tl, br );
	printData();
}

////////////////////////////////////////////////////////////////////////////////
///						draw every line in the QuadTree						 ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::drawLines( byte **&gray ) const
{
	addLines( gray, root );
}

////////////////////////////////////////////////////////////////////////////////
///		draw the lines as directed by the dimenstions stored in each node	 ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::addLines( byte **&gray, const node *quadrant ) const // rapid protoyping: lines could be stored externally from the quadTree node structure
{
	unsigned i;

	if( quadrant == nullptr )
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

////////////////////////////////////////////////////////////////////////////////
///			return the compressed image constructed from the QuadTree		 ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::getCompressedImage( byte **&gray ) const
{
	buildCompressedImage( gray, root );
}

////////////////////////////////////////////////////////////////////////////////
///			construct the compressed image from the QuadTree object			 ///
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
///		subdivide the image to construct the QuadTree object recursively	 ///
////////////////////////////////////////////////////////////////////////////////
QuadTree::node *QuadTree::subdivide( byte **&gray,
		pair<unsigned, unsigned> topLeft, pair<unsigned, unsigned> bottomRight )
{
	node *quadrant = new node( topLeft, bottomRight,
			evalSubdivision( gray, topLeft, bottomRight ) );
	pair<unsigned, unsigned> center = { topLeft.first +
		( bottomRight.first - topLeft.first ) / 2, topLeft.second +
			( bottomRight.second - topLeft.second ) / 2 };
	pair<unsigned, unsigned> nwtl = topLeft;
	pair<unsigned, unsigned> nwbr = center;
	pair<unsigned, unsigned> swtl = { topLeft.first, center.second };
	pair<unsigned, unsigned> swbr = { center.first, bottomRight.second };
	pair<unsigned, unsigned> netl = { center.first, topLeft.second };
	pair<unsigned, unsigned> nebr = { bottomRight.first, center.second };
	pair<unsigned, unsigned> setl = center;
	pair<unsigned, unsigned> sebr = bottomRight;

	if( !needSubdivide( gray, evalSubdivision( gray, topLeft, bottomRight ),
				topLeft, bottomRight ) || topLeft.first - bottomRight.first < 2 ||
			topLeft.second - bottomRight.second < 2 )
	{
		leafNodeCount++;
		return quadrant;
	}

	quadrant->nw = subdivide( gray, nwtl, nwbr );
	quadrant->sw = subdivide( gray, swtl, swbr );
	quadrant->ne = subdivide( gray, netl, nebr );
	quadrant->se = subdivide( gray, setl, sebr );
	return quadrant;
}

////////////////////////////////////////////////////////////////////////////////
/// subdivide if the difference between the maximum and minimum pixel values ///
/// in the quadrant is greater than the threshold, or if the differnce		 ///
/// between the maximum/minimum pixel value and the parent nodes' pixel		 ///
/// values is greater than the threshold									 ///
////////////////////////////////////////////////////////////////////////////////
bool QuadTree::needSubdivide( byte **&gray, const byte rep,
		const pair<unsigned, unsigned> topLeft,
		const pair<unsigned, unsigned> bottomRight ) const
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
			if( abs( rep - mx ) > threshold )
				return 1;
			if( abs( rep - mn ) > threshold )
				return 1;
		}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// return the mean of the pixel valeus in the quadrant or return 0 if the	 ///
///						quadrant has no width or height						 ///
////////////////////////////////////////////////////////////////////////////////
unsigned QuadTree::evalSubdivision( byte **&gray,
		const pair<unsigned, unsigned> topLeft,
		const pair<unsigned, unsigned> bottomRight ) const
{
	unsigned i, j;
	unsigned sum = 0;
	if( bottomRight.second - topLeft.second == 0 ||
			bottomRight.first - topLeft.first == 0 )
		return 0;
	for( i = topLeft.second; i < bottomRight.second; i++ )
		for( j = topLeft.first; j < bottomRight.first; j++ )
			sum += gray[i][j];
	return sum / ( ( bottomRight.second - topLeft.second ) *
			( bottomRight.first - topLeft.first ) );
}

unsigned QuadTree::leafCount( ) const
{
	return leafNodeCount;
}

////////////////////////////////////////////////////////////////////////////////
///					Outputs imformation about the image						 ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::printData( ) const
{
	/*
	// Calculate compression
	compression = ( 200 * leafNodeCount ) / uncompressedSize;

	byteCount = 56 * leafNodeCount;
	// Output to data to console
	cout << "Leaves = " << leafNodeCount << " mem: " << 2 * leafNodeCount
		<< " bytes: " << byteCount << " compressed size: " << compression
		<<"% : Quality Factor [" << int(threshold) << "]" << endl;
		*/
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
