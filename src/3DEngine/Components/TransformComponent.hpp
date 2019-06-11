#pragma once 
#include "Common/Common.h"
#include "Component.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class Component;
class TransformComponent : public Component {
    public: 
        TransformComponent(Object3D* object);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();

        glm::dvec3 position;
        glm::dvec3 rotation;
        glm::dvec3 scale;

        glm::dmat4 GetModelMatrix(); 
};
}
}