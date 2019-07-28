#pragma once

#include "../Object3D.hpp"
#include "Common/Common.h"


namespace KikooRenderer {
namespace SpatialPartition {

struct Node {
    Node* parent;
    std::vector<Node*> children;
    std::vector<CoreEngine::Object3D*> objects;
    glm::dvec3 position;
    glm::dvec3 size;
};

class Octree {
    public:
        Octree();
        void Build(std::vector<CoreEngine::Object3D*> objects, int maxLevel);
        

        //When check 
        //Check first 8 cubes
        //If cube intersects 
            //Check children cubes
        //Return all cubes that intersected



        std::vector<Node> nodes;
};
}
}