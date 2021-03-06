#pragma once

#include "Util/Common.h"
#include "3DEngine/Object3D.hpp"


namespace KikooRenderer {
namespace SpatialPartition {

struct Node {
    Node* parent;
    std::vector<Node*> children;
    std::vector<CoreEngine::Object3D*> objects;
    glm::vec3 position;
    glm::vec3 size;
	int level;
};

class Octree {
    public:
        Octree();
        void Build(CoreEngine::Scene* scene, std::vector<CoreEngine::Object3D*> objects, int maxLevel);
        

        //When check 
        //Check first 8 cubes
        //If cube intersects 
            //Check children cubes
        //Return all cubes that intersected



        std::vector<Node> nodes;	

private:
	int maxLevel;
	void Subdivide(Node node);
	CoreEngine::Scene* scene;

	bool isInside(Node node, CoreEngine::Object3D* object);
};
}
}