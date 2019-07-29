#include "Octree.hpp"

namespace KikooRenderer {
namespace SpatialPartition {

Octree::Octree() {

}

void Octree::Build(std::vector<CoreEngine::Object3D*> objects, int maxLevel) {
    //Foreach level
        //Foreach existing box
            //Subdivide it in 8
                //Check if parents objects are inside
    //Remove empty nodes
}

}
}