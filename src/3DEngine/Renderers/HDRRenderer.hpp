#pragma once
#include "Util/Common.h"
#include "Renderer.hpp"
#include "../Framebuffer.hpp"
#include "../Object3D.hpp"
#include "../Shader.hpp"

namespace KikooRenderer {
namespace CoreEngine {

class HDRRenderer : public Renderer {
public:
    HDRRenderer(Scene* scene);
    virtual void Render();
    virtual void Resize(int w, int h);
    virtual void SetMSAA(bool value);
    
	Framebuffer* quadFBO;
	Framebuffer* alternateFBO;

    int width, height;
    float exposure;
	Object3D* quad;
	Object3D* dummyQuad;

    Shader quadShader;

    bool useMSAA = true;
// protected:
    // void InitShader();
};

}
}