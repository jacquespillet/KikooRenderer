#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"

namespace KikooRenderer {

namespace CoreEngine {
class Component;
class TransformComponent;
class Scene;
class Object3D {
    public: 
        std::string name;
        std::vector<Component*> components;
		TransformComponent* transform;

        Scene* scene;
        std::vector<Object3D*> childObjects;
        Object3D* parent = nullptr;


        bool visible = true;
        bool enabled;
        bool started;
        bool depthTest = true;
		bool isStatic = true;

        bool hasToRecompute=false;

        bool isSelected = false;

        Object3D(std::string name, Scene* scene);
        void AddComponent(Component* component);
        std::string AddObject(Object3D* object);
		void ClearObjects();
        Component*  GetComponent(std::string name);

        void Start();
        void Enable();
        void Render(glm::dmat4* overridenViewMat = nullptr, glm::dmat4* overridenProjMat = nullptr);
        void Update();
        void Destroy();
        void Recompute();

        Object3D* Intersects(Geometry::Ray ray, double& distance);

        QJsonObject ToJSON();
};
}

}