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
	QuadTree( byte **&gray, const unsigned width, const unsigned height,
			const byte thresh );
	~QuadTree( );

	void decrementThreshold( byte **&gray, const unsigned width,
			const unsigned height );
	void incrementThreshold( byte **&gray, const unsigned width,
			const unsigned height );

	unsigned leafCount( ) const;

	void getCompressedImage( byte **&gray ) const;

	void drawLines( byte **&gray ) const;

	unsigned size( ) const;

	byte threshold( ) const;

	private:
	node *subdivide( byte **&gray, pair<unsigned, unsigned> topLeft,
			pair<unsigned, unsigned> bottomRight );

	byte evalSubdivision( byte **&gray,
			const pair<unsigned, unsigned> topLeft,
			const pair<unsigned, unsigned> bottomRight, bool need ) const;

	void insert( byte **&gray, node *quadrant );
	void remove( byte **&gray, node *quadrant );

	void buildCompressedImage( byte **&gray, const node *quadrant ) const;
	void addLines( byte **&gray, const node *quadrant ) const;

	vector<pair<unsigned, unsigned>> calculateChildBoundryPoints(
			pair<unsigned, unsigned> topLeft,
			pair<unsigned, unsigned> bottomRight ) const;

	pair<unsigned, unsigned> center( pair<unsigned, unsigned> topLeft,
			pair<unsigned, unsigned> bottomRight ) const;


	void clear( node *n );
};
#endif
