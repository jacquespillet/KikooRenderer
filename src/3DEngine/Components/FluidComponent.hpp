#pragma once
#include "BoundingComponent.hpp"
#include "Component.hpp"
#include "../Object3D.hpp"

#include "Util/Common.h"

namespace KikooRenderer {
namespace CoreEngine {
    class FluidComponent : public Component {
        public:
        FluidComponent(Object3D* object);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
        void Recompute(){}

        MeshFilterComponent* meshFilter;

        int width = 100;
        int height = 100;
        int numVert;

        int renderBuffer=0;

        std::vector<glm::dvec3> buffer[2];
        std::vector<glm::dvec3> normal;
        std::vector<glm::dvec3> tangent;

        float t = 0.1; //time interval
        float c = 0.4; //velocity
        float d = 0.1; //Distance btw vert
        float mu = 0.01;

        float k1, k2, k3;
    };
}
}