#pragma once
#include "Util/Common.h"
#include "Renderer.hpp"
#include "../Framebuffer.hpp"
#include "../Object3D.hpp"
#include "../Shader.hpp"

#include "../PostProcessing/PostProcessor.hpp"
#include "../ParticleSystem/ParticleSystem.hpp"

namespace KikooRenderer {
namespace CoreEngine {

class HDRRenderer : public Renderer {
public:
    HDRRenderer(Scene* scene);
    virtual void Render();
    virtual void Resize(int w, int h);
    virtual void SetMSAA(bool value);
    virtual void Destroy();
    virtual void AddPostEffect(PostProcess* postProcess);
    virtual void RemovePostEffect(PostProcess* postProcess);

    void SetFramebuffers();

	Framebuffer* quadFBO;
	Framebuffer* alternateFBO;
	Framebuffer* finalFBO;

    int width, height;
    float exposure;
	Object3D* quad;
	Object3D* dummyQuad;

    Shader quadShader;

    bool useMSAA = true;

    PostProcessor postProcessor;
// protected:
    // void InitShader();
};

}
}