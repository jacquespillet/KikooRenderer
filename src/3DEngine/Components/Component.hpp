#pragma once
#include "Common/Common.h"
// #include "Object3D.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class Object3D;
class Component  {
    public: 
        std::string name;
        Object3D* object3D;

        Component(std::string name, Object3D* object);
        virtual void OnStart()=0;
        virtual void OnEnable()=0;
        virtual void OnUpdate()=0;
        virtual void OnRender()=0;
        virtual void OnDestroy()=0;
        virtual void Recompute()=0;

        bool inited;
    protected: 
};

}
}