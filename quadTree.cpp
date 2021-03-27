#include "quadTree.h"
#include "alloc2d.h"

using namespace std;



/*  ____       _            _         ___       _             __
   |  _ \ _ __(_)_   ____ _| |_ ___  |_ _|_ __ | |_ ___ _ __ / _| __ _  ___ ___
   | |_) | '__| \ \ / / _` | __/ _ \  | || '_ \| __/ _ \ '__| |_ / _` |/ __/ _ \
   |  __/| |  | |\ V / (_| | ||  __/  | || | | | ||  __/ |  |  _| (_| | (_|  __/
   |_|   |_|  |_| \_/ \__,_|\__\___| |___|_| |_|\__\___|_|  |_|  \__,_|\___\___|
 */



////////////////////////////////////////////////////////////////////////////////
/// subdivide the image to construct the QuadTree node structure recursively ///
////////////////////////////////////////////////////////////////////////////////
QuadTree::node *QuadTree::subdivide( byte **&gray,
        pair<unsigned, unsigned> topLeft, pair<unsigned, unsigned> bottomRight )
{
    vector<pair<unsigned, unsigned>> childBoundryPoints =
        calculateChildBoundryPoints( topLeft, bottomRight );

    node *quadrant = new node( topLeft, bottomRight,
            evalSubdivision( gray, topLeft, bottomRight ) );
    nodeCount++;

    if( !needSubdivide( gray, topLeft, bottomRight ) ||
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
///     return the mean of a quadrant for the pixelValue of the quadrant     ///
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



////////////////////////////////////////////////////////////////////////////////
/// return a bool indicating if a subdivision is necessary with the current  ///
/// threshold, a subdivision is necessary if the difference between the      ///
/// maximum/minimum values in the quadrant and the average of the quadrant's ///
/// values is greater than the threshold                                     ///
////////////////////////////////////////////////////////////////////////////////
bool QuadTree::needSubdivide( byte **&gray,
        const pair<unsigned, unsigned> topLeft,
        const pair<unsigned, unsigned> bottomRight ) const
{
    unsigned i, j;
    unsigned mean = evalSubdivision( gray, topLeft, bottomRight );
    byte mx = gray[ topLeft.second ][ topLeft.first ];
    byte mn = gray[ topLeft.second ][ topLeft.first ];
    for( i = topLeft.second; i < bottomRight.second; i++ )
        for( j = topLeft.first; j < bottomRight.first; j++ )
        {
            if( gray[i][j] > mx )
                mx = gray[i][j];
            if( gray[i][j] < mn )
                mn = gray[i][j];
        }
    if( mx - mean <= tolerance && mean - mn <= tolerance )
        return 0;
    return 1;
}



////////////////////////////////////////////////////////////////////////////////
///         construct the compressed image from the QuadTree object          ///
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
///     draw the lines as directed by the dimenstions stored in the leaves   ///
///     node                                                                 ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::addLines( byte **&gray, const node *quadrant ) const
{
    unsigned i;

    if( quadrant->isLeaf( ) )
    {
        for( i = quadrant->topLeft.second; i < quadrant->bottomRight.second; i++ )
            gray[i][ quadrant->topLeft.first ] = 255;
        for( i = quadrant->topLeft.first; i < quadrant->bottomRight.first; i++ )
            gray[ quadrant->topLeft.second ][i] = 255;
        return;
    }

    addLines( gray, quadrant->nw );
    addLines( gray, quadrant->ne );
    addLines( gray, quadrant->sw );
    addLines( gray, quadrant->se );
}



////////////////////////////////////////////////////////////////////////////////
/// updates QuadTree by inserting new nodes on account of updated threshold  ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::insert( byte **&gray, node *quadrant )
{
    vector<pair<unsigned, unsigned>> childBoundryPoints =
        calculateChildBoundryPoints( quadrant->topLeft, quadrant->bottomRight );

    if( quadrant->isLeaf( ) )
    {
        if( needSubdivide( gray, quadrant->topLeft, quadrant->bottomRight ) )
        {
            quadrant->nw = subdivide( gray, childBoundryPoints[0], childBoundryPoints[1] );
            quadrant->sw = subdivide( gray, childBoundryPoints[2], childBoundryPoints[3] );
            quadrant->ne = subdivide( gray, childBoundryPoints[4], childBoundryPoints[5] );
            quadrant->se = subdivide( gray, childBoundryPoints[6], childBoundryPoints[7] );
            leafNodeCount--;
        }
        return;
    }

    insert( gray, quadrant->nw );
    insert( gray, quadrant->sw );
    insert( gray, quadrant->ne );
    insert( gray, quadrant->se );
}



////////////////////////////////////////////////////////////////////////////////
///       updates QuadTree on account of new threshold by removing nodes     ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::remove( byte **&gray, node *quadrant )
{
    if( !needSubdivide( gray, quadrant->topLeft, quadrant->bottomRight ) )
    {
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
///               return the number of leaves in the subTree                 ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::subTreeLeafNodeCount( node *quadrant, unsigned &newLeafNodeCount ) const
{
    if( quadrant->isLeaf( ) )
    {
        newLeafNodeCount++;
        return;
    }

    subTreeLeafNodeCount( quadrant->nw, newLeafNodeCount );
    subTreeLeafNodeCount( quadrant->sw, newLeafNodeCount );
    subTreeLeafNodeCount( quadrant->ne, newLeafNodeCount );
    subTreeLeafNodeCount( quadrant->se, newLeafNodeCount );
}



////////////////////////////////////////////////////////////////////////////////
/// returns the top-left and bottom-right points of all 4 children quadrants ///
/// given the top-left and bottom-right of the parent                        ///
////////////////////////////////////////////////////////////////////////////////
vector<pair<unsigned, unsigned>> QuadTree::calculateChildBoundryPoints(
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
///                           bottom-right points                            ///
////////////////////////////////////////////////////////////////////////////////
pair<unsigned, unsigned> QuadTree::center( pair<unsigned, unsigned> topLeft,
        pair<unsigned, unsigned> bottomRight ) const
{
    return { topLeft.first + ( bottomRight.first - topLeft.first ) / 2,
        topLeft.second + ( bottomRight.second - topLeft.second ) / 2 };
}



////////////////////////////////////////////////////////////////////////////////
/// recusively deletes nodes from subtree ( n ) while managing the nodeCount ///
////////////////////////////////////////////////////////////////////////////////
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



/*   ____        _     _ _        ___       _             __
    |  _ \ _   _| |__ | (_) ___  |_ _|_ __ | |_ ___ _ __ / _| __ _  ___ ___
    | |_) | | | | '_ \| | |/ __|  | || '_ \| __/ _ \ '__| |_ / _` |/ __/ _ \
    |  __/| |_| | |_) | | | (__   | || | | | ||  __/ |  |  _| (_| | (_|  __/
    |_|    \__,_|_.__/|_|_|\___| |___|_| |_|\__\___|_|  |_|  \__,_|\___\___|
 */



////////////////////////////////////////////////////////////////////////////////
///         Construct a new QuadTree given an image and a threshold          ///
/// throws an exception of value 1 given an invalid image, width, or height  ///
////////////////////////////////////////////////////////////////////////////////
QuadTree::QuadTree( byte **&gray, const unsigned width, const unsigned height,
        const byte thresh ) : tolerance( thresh % 129 ), nodeCount( 0 ), leafNodeCount( 0 )
// threshold instantiated with value [0 - 128] because a threshold outside the
// range [0 - 128] is not applicable for the application since this is the
// limitation of a pixel value ( 255 ) ( e.a. a threshold of 128 is equivalent to any
// threshold greater than 128 in implementation )
{
    if( gray == nullptr || !width || !height )
        throw 1;
    pair<unsigned, unsigned> tl = { 0, 0 };
    pair<unsigned, unsigned> br = { width, height };
    root = subdivide( gray, tl, br );
}



////////////////////////////////////////////////////////////////////////////////
///         return the compressed image constructed from the QuadTree        ///
////////////////////////////////////////////////////////////////////////////////
byte **QuadTree::getCompressedImage( ) const
{
    byte **gray = alloc2D_byte( root->bottomRight.first, root->bottomRight.second );
    buildCompressedImage( gray, root );
    return gray;
}



////////////////////////////////////////////////////////////////////////////////
///                     draw every line in the QuadTree                      ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::drawLines( byte **&gray ) const
{
    addLines( gray, root );
}



////////////////////////////////////////////////////////////////////////////////
///        delete nodes while possible to maintain the looser threshold      ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::incrementThreshold( byte **&gray )
{
    unsigned newLeafNodeCount = 0;
    if( tolerance > 127 )
        return;
    tolerance++;
    remove( gray, root );
    subTreeLeafNodeCount( root, newLeafNodeCount );
    leafNodeCount = newLeafNodeCount;
}



////////////////////////////////////////////////////////////////////////////////
///        insert nodes as necessary to maintain the tighter threshold       ///
////////////////////////////////////////////////////////////////////////////////
void QuadTree::decrementThreshold( byte **&gray )
{
    if( tolerance < 1 )
        return;
    tolerance--;
    insert( gray, root );
}



unsigned QuadTree::size( ) const
{
    return nodeCount;
}



unsigned QuadTree::leafCount( ) const
{
    return leafNodeCount;
}



byte QuadTree::threshold( ) const
{
    return tolerance;
}



QuadTree::~QuadTree( )
{
    clear( root );
    delete root;
}
