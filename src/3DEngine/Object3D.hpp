#pragma once
#include "Common/Common.h"
// #include "Component.hpp"
#include "Scene.hpp"

namespace KikooRenderer {

namespace CoreEngine {
class Component;
class Scene;
class Object3D {
    public: 
        std::string name;
        std::vector<Component*> components;
        Scene* scene;
        std::vector<Object3D*> childObjects;

        bool visible;
        bool enabled;

        Object3D(std::string name, Scene* scene);
        void AddComponent(Component* component);
        void AddObject(Object3D* object);
        Component*  GetComponent(std::string name);

        void Start();
        void Enable();
        void Render(glm::mat4* modelMatrix=nullptr);
        void Update();
        void Destroy();
};
}

}