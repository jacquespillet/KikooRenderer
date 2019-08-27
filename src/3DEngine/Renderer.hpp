#pragma once
#include "Scene.hpp"

namespace KikooRenderer {
namespace CoreEngine {

class Renderer {
public:
    Renderer(Scene* scene);
    virtual void Render();
protected:
    Scene* scene;
};

class ForwardRenderer : public Renderer {
public:
    ForwardRenderer(Scene* scene);
    virtual void Render();
};

class HDRRenderer : public Renderer {
public:
    HDRRenderer(Scene* scene);
    virtual void Render();
    
	Framebuffer* alternateFBO;
	Object3D* quad;
};

}
}