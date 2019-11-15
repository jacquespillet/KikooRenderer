#include "BVH.hpp"


namespace KikooRenderer {
namespace OfflineRenderer {
    BVH::BVH(std::vector<Shape*>* shapes) {
        this->shapes = shapes;
    }

    void BVH::RecursiveBuild(int start, int end) {
        //Create node
        //numNodes++;
        //Compute bounds of all primitives
        //for start to end
        //  Bounds from all these
        //int nPrimitives = end - start
        //if nprimitive == 1
        //  Create LeafNode : 
        //  orderedPrimitives.push(start)
        //  node->InitLeaf(start, nPrimitives, bounds)
        //  return node
        //Else
        //  compute bound of primitives
        //  choose spliting dimension (maximum extent of the bound)
        //  Partition primitives into two sets
        //      Compute middle index
    //          
        //  builds children
    }
}
}