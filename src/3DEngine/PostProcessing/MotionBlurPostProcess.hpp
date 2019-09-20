#pragma once
#include "Util/Common.h"
#include "../Framebuffer.hpp"
#include "../Shader.hpp"
#include "PostProcess.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class MaterialComponent;
class MotionBlurPostProcess: public PostProcess  {
public:
    MotionBlurPostProcess(Scene* scene);
    virtual void Run(Framebuffer* famebufferIn, Framebuffer* famebufferOut);

    Shader shader;
	Object3D* quad;
    MaterialComponent* material;
    Texture albedoTex;

    float velocityMultiplier = 0.125;
    int numSamples = 4;
};
}
}