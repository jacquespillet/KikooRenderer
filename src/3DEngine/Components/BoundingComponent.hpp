#pragma once
#include "BoundingComponent.hpp"
#include "Component.hpp"
#include "../Object3D.hpp"

#include "Common/Common.h"

namespace KikooRenderer {
namespace CoreEngine {
    // //Maybe use it when different types of components ? 
    // class BoundingComponent : public Component {

    // }

    class BoundingBoxComponent : public Component {
        public:
        BoundingBoxComponent(Object3D* object);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
        void Recompute(){}

        glm::dvec3 min;
        glm::dvec3 max;

        Object3D* GetBoxObject();
        private: 
        Object3D* boxObject;
    };
    
    // class BoundingSphere : public Component {
        
    // }

    // class BoundingEllipsoid : public Component {
        
    // }        
}
}