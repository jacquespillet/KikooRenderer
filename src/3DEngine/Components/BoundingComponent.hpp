#pragma once
#include "Util/Common.h"
#include "../Object3D.hpp"


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

        void GetWorldBounds(glm::dvec3* _min, glm::dvec3* _max);
        void GetLocalBounds(glm::dvec3* _min, glm::dvec3* _max);

        glm::dvec3 min;
        glm::dvec3 max;

        glm::dvec3 wmin;
        glm::dvec3 wmax;

		std::vector<glm::dvec3> positions;

        Object3D* GetBoxObject();
        private: 
        Object3D* boxObject;
        TransformComponent* transform;
    };
    
    // class BoundingSphere : public Component {
        
    // }

    // class BoundingEllipsoid : public Component {
        
    // }        
}
}