#pragma once
#include "Util/Common.h"
#include "../Framebuffer.hpp"
#include "../Shader.hpp"
#include "PostProcess.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class MaterialComponent;
class BloomPostProcess: public PostProcess  {
public:
    BloomPostProcess(Scene* scene);
    virtual void Run(Framebuffer* famebufferIn, Framebuffer* famebufferOut);
    virtual void OnResize(int w, int h);

    Framebuffer* thresholdFramebuffer;
    Framebuffer* alternateFramebuffer;

    Shader thresholdShader;
    Shader blurShader;
    Shader blendShader;

    float brightnessThreshold = 0.3;
    int kernelSize = 5;
    float kernelRadius = 1.0;

	Object3D* quad;
    MaterialComponent* material;
    Texture albedoTex;
};
}
}