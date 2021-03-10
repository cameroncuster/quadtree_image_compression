#ifndef QUADTREE_H
#define QUADTREE_H
#include "point.h"
#include <vector>
#include <utility>

using namespace std;

class QuadTree
{
	unsigned char margin;
	struct node
	{
		Point topLeft;
		Point bottomRight;
		unsigned char pixelValue;
		node* nw;
		node* sw;
		node* ne;
		node* se;

		node( Point tl, unsigned char pixVal, Point br,
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
	QuadTree( unsigned char **gray, unsigned char tolerance, unsigned width, unsigned height );
	~QuadTree( );

	unsigned char **getImage( ) const;
	vector<pair<unsigned, unsigned>> getImageBorders( ) const;

	private:
	void subdivide( unsigned char **gray, node *quadrant );
	bool needSubdivide( unsigned char **gray, node *quadrant ) const;
	unsigned evalSubdivision( unsigned char **gray, node *quadrant ) const;
	unsigned char **buildImage( );
};
#endif
