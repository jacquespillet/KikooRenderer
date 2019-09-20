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

    Framebuffer* thresholdFramebuffer;
    Framebuffer* alternateFramebuffer;

    Shader thresholdShader;
    Shader blurShader;
    Shader blendShader;

	Object3D* quad;
    MaterialComponent* material;
    Texture albedoTex;
};
}
}