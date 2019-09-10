#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"

namespace KikooRenderer {

namespace CoreEngine {
class Component;
class TransformComponent;
class LightComponent;
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

        bool castShadow = true;

        Object3D(std::string name, Scene* scene);
        ~Object3D();

        void AddComponent(Component* component);
        std::string AddObject(Object3D* object);
		void ClearObjects();
        Component*  GetComponent(std::string name);

        void Start();
        void Enable();
        void Render();
        void DepthRenderPass(LightComponent* light);
        void Update();
        void Destroy();
        void Recompute();

        Object3D* Intersects(Geometry::Ray ray, double& distance);

        QJsonObject ToJSON();
};
}

}