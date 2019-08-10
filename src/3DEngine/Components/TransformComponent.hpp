#pragma once 
#include "Util/Common.h"
#include "Component.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class Object3D;

class TransformComponent : public Component {
    public: 
        TransformComponent(Object3D* object);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
		void Recompute();

        glm::dvec3 position;
        glm::dvec3 rotation;
        glm::dvec3 scale;

        glm::dmat4 GetTransRotMatrix(); 
        glm::dmat4 GetModelMatrix(); 
        glm::dvec3 GetScale(); 
        
        glm::mat4 GetWorldModelMatrix();
        glm::dmat4 GetWorldTransRotMatrix(); 
		glm::dvec3 GetWorldPosition();
		glm::dvec3 GetWorldScale();

		void SetWorldX(double x);
		void SetWorldY(double y);
		void SetWorldZ(double z);

        bool isScreenSize = false;
};
}
}