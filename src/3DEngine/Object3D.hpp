#pragma once
#include "Common/Common.h"
// #include "Component.hpp"
#include "Scene.hpp"
#include "Geometry/Ray.hpp"
#include "Components/TransformComponent.hpp"
#include "Framebuffer.hpp"

namespace KikooRenderer {

namespace CoreEngine {
class Component;
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

        bool hasToRecompute=false;

        bool isSelected = false;

        Object3D(std::string name, Scene* scene);
        void AddComponent(Component* component);
        std::string AddObject(Object3D* object);
        Component*  GetComponent(std::string name);

        void Start();
        void Enable();
        void Render(glm::mat4* modelMatrix=nullptr);
        void Update();
        void Destroy();
        void Recompute();

        Object3D* Intersects(Geometry::Ray ray, double& distance);

		//Useful ? 
		//Framebuffer* frameBuffer;
};
}

}