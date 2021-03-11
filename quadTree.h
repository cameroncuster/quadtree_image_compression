#ifndef QUADTREE_H
#define QUADTREE_H
#include <vector>
#include <utility>

using namespace std;

class QuadTree
{
	unsigned char margin;
	struct node
	{
		pair<unsigned, unsigned> topLeft;
		pair<unsigned, unsigned> bottomRight;
		unsigned char pixelValue;
		node* nw;
		node* sw;
		node* ne;
		node* se;

		node( pair<unsigned, unsigned> tl, pair<unsigned, unsigned> br, unsigned char pixVal,
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
	QuadTree( unsigned char **gray, const unsigned char tolerance, const unsigned width, const unsigned height );
	~QuadTree( );

	unsigned char **draw( const bool lines ) const;

	private:
	void subdivide( unsigned char **&gray, node *quadrant );
	bool needSubdivide( unsigned char **&gray, const unsigned char rep, const pair<unsigned, unsigned> topLeft, const pair<unsigned, unsigned> bottomRight ) const;
	unsigned evalSubdivision( unsigned char **&gray, const pair<unsigned, unsigned> topLeft, const pair<unsigned, unsigned> bottomRight ) const;
	void drawImage( unsigned char **&gray, const node *quadrant, const bool lines ) const;

	void clear( node *n );
};
#endif
