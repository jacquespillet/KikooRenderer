#pragma once
#include "Util/Common.h"
#include "../Framebuffer.hpp"
#include "../Shader.hpp"
#include "PostProcess.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class MaterialComponent;
class DepthOfFieldPostProcess: public PostProcess  {
public:
    DepthOfFieldPostProcess(Scene* scene);
    virtual void Run(Framebuffer* famebufferIn, Framebuffer* famebufferOut);

    Shader shader;
	Object3D* quad;
    MaterialComponent* material;
    Texture albedoTex;

    float bokehRadius = 8.0;
    float focusDistance = 10.0;
    float focusRange = 3.0;
};
}
}