#ifndef QUADTREE_H
#define QUADTREE_H
#include <vector>
#include <utility>

using namespace std;

typedef unsigned char byte;

class QuadTree
{
	byte threshold;
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
	};
	node *root;

	unsigned pixelCount;
	unsigned nodeCount;
	unsigned leafNodeCount;
	unsigned byteCount; // malloc but don't free and ask Valgrind how much I used...
	unsigned compression;

	public:
	QuadTree( byte **gray, const unsigned width, const unsigned height, const byte thresh );
	~QuadTree( );

	void decreaseThreshold( );
	void increaseThreshold( );

	unsigned leafCount( ) const;

	byte **getCompressedImage( ) const;

	private:
	node *subdivide( byte **&gray, pair<unsigned, unsigned> topLeft, pair<unsigned, unsigned> bottomRight );
	bool needSubdivide( byte **&gray, const byte rep, const pair<unsigned, unsigned> topLeft, const pair<unsigned, unsigned> bottomRight ) const;
	unsigned evalSubdivision( byte **&gray, const pair<unsigned, unsigned> topLeft, const pair<unsigned, unsigned> bottomRight ) const;
	void buildCompressedImage( byte **&gray, const node *quadrant ) const;

	void clear( node *n );
};
#endif
