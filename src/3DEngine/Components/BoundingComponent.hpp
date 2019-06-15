#pragma once
#include "BoundingComponent.hpp"
#include "Component.hpp"
#include "../Object3D.hpp"

#include "Common/Common.h"

namespace KikooRenderer {
namespace CoreEngine {
    class BoundingBoxComponent : public Component {
        public:
        BoundingBoxComponent(Object3D* object);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();

        glm::dvec3 min;
        glm::dvec3 max;

        //
        // glm::dvec3 frontTopLeft;
        // glm::dvec3 frontTopRight;
        // glm::dvec3 frontBottomRight;
        // glm::dvec3 frontBottomLeft;

        // glm::dvec3 backTopLeft;
        // glm::dvec3 backTopRight;
        // glm::dvec3 backBottomRight;
        // glm::dvec3 backBottomLeft;
        // Collides(Ray ray);     
    };
    
    // class BoundingSphere : public Component {
        
    // }

    // class BoundingEllipsoid : public Component {
        
    // }        
}
}