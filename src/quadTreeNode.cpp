#include "inc/quadTree.h"

bool QuadTree::node::isLeaf( ) const
{
    return nw == nullptr && sw == nullptr && ne == nullptr && se == nullptr;
}
