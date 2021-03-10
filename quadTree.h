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
		pair<unsigned, unsigned> point;
		unsigned char pixelValue;
		unsigned height, width;
		node *children[4];
	};
	node *root;

	unsigned pixelCount;
	unsigned nodeCount;
	unsigned leafNodeCount;
	unsigned byteCount; // malloc but don't free and ask Valgrind how much I used...
	unsigned compression;

	vector<pair<unsigned, unsigned>> borders;

	public:
	QuadTree( unsigned char **gray, unsigned char magin, unsigned width, unsigned height );
	~QuadTree( );

	unsigned char **getImage( ) const;
	vector<pair<unsigned, unsigned>> getImageBorders( ) const;

	private:
	void buildTree( unsigned char **gray, unsigned char margin, unsigned width, unsigned height );
	unsigned char **buildImage( );
};
#endif
