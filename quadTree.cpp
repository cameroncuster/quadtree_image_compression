#include "quadTree.h"
#include "alloc2d.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
///			Construct a new QuadTree given an image and a threshold			 ///
////////////////////////////////////////////////////////////////////////////////
QuadTree::QuadTree( byte **&gray, const unsigned width, const unsigned height,
		const byte thresh ) : tolerance( thresh ), nodeCount( 0 ), leafNodeCount( 0 )
{
	if( gray == nullptr || !width || !height )
		throw;
	pair<unsigned, unsigned> tl = { 0, 0 };
	pair<unsigned, unsigned> br = { width, height };
	root = subdivide( gray, tl, br );
}

QuadTree::~QuadTree( )
{
	clear( root );
	delete root;
}

////////////////////////////////////////////////////////////////////////////////
///		   insert nodes as necessary to maintain the tighter threshold		 ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::decrementThreshold( byte **&gray, const unsigned width,
		const unsigned height )
{
	if( tolerance < 1 )
		return;
	tolerance--;
	insert( gray, root );
}

////////////////////////////////////////////////////////////////////////////////
/// updates QuadTree by inserting new nodes on account of updated threshold  ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::insert( byte **&gray, node *quadrant )
{
	vector<pair<unsigned, unsigned>> childBoundryPoints =
		getChildBoundryPoints( quadrant->topLeft, quadrant->bottomRight );

	if( quadrant->isLeaf( ) )
	{
		if( evalSubdivision( gray, quadrant->topLeft, quadrant->bottomRight, 1 ) )
		{
			quadrant->nw = subdivide( gray, childBoundryPoints[0], childBoundryPoints[1] );
			quadrant->sw = subdivide( gray, childBoundryPoints[2], childBoundryPoints[3] );
			quadrant->ne = subdivide( gray, childBoundryPoints[4], childBoundryPoints[5] );
			quadrant->se = subdivide( gray, childBoundryPoints[6], childBoundryPoints[7] );
		}
		return;
	}

	insert( gray, quadrant->nw );
	insert( gray, quadrant->sw );
	insert( gray, quadrant->ne );
	insert( gray, quadrant->se );
}

////////////////////////////////////////////////////////////////////////////////
///		   delete nodes while possible to maintain the looser threshold		 ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::incrementThreshold( byte **&gray, const unsigned width,
		const unsigned height )
{
	if( tolerance > 255 )
		return;
	tolerance++;
	remove( gray, root );
}

////////////////////////////////////////////////////////////////////////////////
///		  updates QuadTree on account of new threshold by removing nodes	 ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::remove( byte **&gray, node *quadrant )
{
	if( !evalSubdivision( gray, quadrant->topLeft, quadrant->bottomRight, 1 ) )
	{
		// udpate leaf node count
		clear( quadrant->nw );
		clear( quadrant->sw );
		clear( quadrant->ne );
		clear( quadrant->se );

		quadrant->nw = nullptr;
		quadrant->sw = nullptr;
		quadrant->ne = nullptr;
		quadrant->se = nullptr;

		return;
	}

	remove( gray, quadrant->nw );
	remove( gray, quadrant->sw );
	remove( gray, quadrant->ne );
	remove( gray, quadrant->se );
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
void QuadTree::addLines( byte **&gray, const node *quadrant ) const
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

	// return if quadrant isLeaf( )

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
	if( quadrant->isLeaf( ) )
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
	vector<pair<unsigned, unsigned>> childBoundryPoints =
		getChildBoundryPoints( topLeft, bottomRight );

	nodeCount++;
	node *quadrant = new node( topLeft, bottomRight,
			evalSubdivision( gray, topLeft, bottomRight, 0 ) );

	if( !evalSubdivision( gray, topLeft, bottomRight, 1 ) ||
			topLeft.first - bottomRight.first < 2 ||
			topLeft.second - bottomRight.second < 2 )
	{
		leafNodeCount++;
		return quadrant;
	}

	quadrant->nw = subdivide( gray, childBoundryPoints[0], childBoundryPoints[1] );
	quadrant->sw = subdivide( gray, childBoundryPoints[2], childBoundryPoints[3] );
	quadrant->ne = subdivide( gray, childBoundryPoints[4], childBoundryPoints[5] );
	quadrant->se = subdivide( gray, childBoundryPoints[6], childBoundryPoints[7] );

	return quadrant;
}

////////////////////////////////////////////////////////////////////////////////
/// subdivide if the difference between the maximum or minimum pixel values  ///
/// in the quadrant and the mean of the quadrant is greater than the		 ///
/// threshold																 ///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// returns the mean of the pixel values in the quadrant or return 0 if the	 ///
///						quadrant has no width or height						 ///
////////////////////////////////////////////////////////////////////////////////
byte QuadTree::evalSubdivision( byte **&gray,
		const pair<unsigned, unsigned> topLeft,
		const pair<unsigned, unsigned> bottomRight, bool need ) const
{
	unsigned i, j;
	unsigned sum = 0;
	byte mean;
	byte mx = gray[ topLeft.second ][ topLeft.first ];
	byte mn = gray[ topLeft.second ][ topLeft.first ];
	if( bottomRight.second - topLeft.second == 0 ||
			bottomRight.first - topLeft.first == 0 )
		return 0;
	for( i = topLeft.second; i < bottomRight.second; i++ )
		for( j = topLeft.first; j < bottomRight.first; j++ )
		{
			sum += gray[i][j];
			if( gray[i][j] > mx )
				mx = gray[i][j];
			if( gray[i][j] < mn )
				mn = gray[i][j];
		}
	mean = sum / ( ( bottomRight.second - topLeft.second ) *
			( bottomRight.first - topLeft.first ) ) + 0.5;
	if( need && mx - mean <= tolerance && mean - mn <= tolerance )
		return 0;
	return mean;
}

unsigned QuadTree::leafCount( ) const
{
	return leafNodeCount;
}

unsigned QuadTree::size( ) const
{
	return nodeCount;
}

byte QuadTree::threshold( ) const
{
	return tolerance;
}

////////////////////////////////////////////////////////////////////////////////
/// returns the top-left and bottom-right points of all 4 children quadrants ///
/// given the top-left and bottom-right of the parent						 ///
////////////////////////////////////////////////////////////////////////////////
vector<pair<unsigned, unsigned>> QuadTree::getChildBoundryPoints(
		pair<unsigned, unsigned> topLeft,
		pair<unsigned, unsigned> bottomRight ) const
{
	pair<unsigned, unsigned> c = center( topLeft, bottomRight );

	return { topLeft, c, { topLeft.first, c.second },
		{ c.first, bottomRight.second }, { c.first, topLeft.second },
		{ bottomRight.first, c.second }, c, bottomRight };
}

////////////////////////////////////////////////////////////////////////////////
/// returns the center of a given quadrant with respect to the top-left and  ///
///							  bottom-right points							 ///
////////////////////////////////////////////////////////////////////////////////
pair<unsigned, unsigned> QuadTree::center( pair<unsigned, unsigned> topLeft,
		pair<unsigned, unsigned> bottomRight ) const
{
	return { topLeft.first + ( bottomRight.first - topLeft.first ) / 2,
		topLeft.second + ( bottomRight.second - topLeft.second ) / 2 };
}

void QuadTree::clear( node *n )
{
	if( n == nullptr )
	{
		nodeCount--;
		delete n;
		return;
	}

	clear( n->nw );
	clear( n->sw );
	clear( n->ne );
	clear( n->se );
}
