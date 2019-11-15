#pragma once
#include "../Shapes/Shape.hpp"
#include "Bounds.hpp"
namespace KikooRenderer {
namespace OfflineRenderer {
class BVH  {
    public:

        struct BVHNode {
            Bounds bounds;
            BVHNode* children[2];
            int splitAxis;  
            int firstPrimOffset;  
            int nPrimitives;  

            void InitInterior(int axis, BVHNode* n0, BVHNode* n1) {
                children[0] = n0;
                children[1] = n1;

                bounds = Bounds(n0->bounds, n1->bounds);
                splitAxis = axis;
                nPrimitives = 0;
            }

            void Initleaf(int first, int n, Bounds _bounds) {
                firstPrimOffset = first;
                nPrimitives = n;
                bounds = _bounds;
                children[0] = nullptr;
                children[1] = nullptr;
            }            
        };

        BVH(std::vector<Shape*>* shapes);

        void RecursiveBuild(int start, int end);
    private:
        std::vector<Shape*>* shapes;
        std::vector<Shape*> orderedShapes;

        int numNodes;
};
}
}