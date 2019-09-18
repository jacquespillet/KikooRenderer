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
};
}
}