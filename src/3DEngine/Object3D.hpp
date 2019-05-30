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

        bool visible;
        bool enabled;

        Object3D(std::string name, Scene* scene);
        void AddComponent(Component* component);
        Component*  GetComponent(std::string name);

        void OnStart();
        void Enable();
        void Render();
};
}

}