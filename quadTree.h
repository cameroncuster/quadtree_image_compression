#ifndef QUADTREE_H
#define QUADTREE_H
#include <vector>
#include <utility>

using namespace std;

typedef unsigned char byte;

class QuadTree
{
	struct node
	{
		pair<unsigned, unsigned> topLeft;
		pair<unsigned, unsigned> bottomRight;
		byte pixelValue;
		node* nw;
		node* sw;
		node* ne;
		node* se;

		node( pair<unsigned, unsigned> tl, pair<unsigned, unsigned> br, byte pixVal,
				node *northwest = nullptr, node *southwest = nullptr,
				node *northeast = nullptr, node *southeast = nullptr ):
			topLeft( tl ), bottomRight( br ), pixelValue( pixVal ),
			nw( northwest ), sw( southwest ), ne( northeast ), se( southeast ) { }

		bool isLeaf( ) const;
	};

	node *root;

	byte tolerance;
	unsigned nodeCount;
	unsigned leafNodeCount;

public:
	// construct a QuadTree given an image and a threshold
	QuadTree( byte **&gray, const unsigned width, const unsigned height,
			const byte thresh );

	// extract the compressed image from the QuadTree
	void getCompressedImage( byte **&gray ) const;

	// draw the quadrant boundry lines on the compressed image
	void drawLines( byte **&gray ) const;

	void incrementThreshold( byte **&gray, const unsigned width,
			const unsigned height );

	void decrementThreshold( byte **&gray, const unsigned width,
			const unsigned height );

	unsigned size( ) const;

	unsigned leafCount( ) const;

	byte threshold( ) const;

	~QuadTree( );

private:
	// subdivide the image into Quadrants and store them as nodes in the QuadTree
	node *subdivide( byte **&gray, pair<unsigned, unsigned> topLeft,
			pair<unsigned, unsigned> bottomRight );

	// determine if a quadrant must be further subdivided and return the mean of
	// the quadrant if necessary
	byte evalSubdivision( byte **&gray,
			const pair<unsigned, unsigned> topLeft,
			const pair<unsigned, unsigned> bottomRight, bool need ) const;

	// construct an image from the QuadTree
	void buildCompressedImage( byte **&gray, const node *quadrant ) const;

	// add the lines to the QuadTree
	void addLines( byte **&gray, const node *quadrant ) const;

	// insert nodes into the QuadTree an account of an decreased threshold
	void insert( byte **&gray, node *quadrant );

	// remove nodes the QuadTree on account of an increased threshold
	void remove( byte **&gray, node *quadrant );

	// evaluate the number of leaf nodes in the subtree ( quadrant )
	void subTreeLeafNodeCount( node *quadrant, unsigned &leafNodeCount ) const;

	// return a vector containing the boundry points of all four children of the
	// quadrant
	vector<pair<unsigned, unsigned>> calculateChildBoundryPoints(
			pair<unsigned, unsigned> topLeft,
			pair<unsigned, unsigned> bottomRight ) const;

	// return the center of a quadrant given the topLeft and bottomRight points
	// of a quadrant
	pair<unsigned, unsigned> center( pair<unsigned, unsigned> topLeft,
			pair<unsigned, unsigned> bottomRight ) const;

	// recursively delete all nodes in the subtree of node ( n )
	void clear( node *n );
};
#endif
