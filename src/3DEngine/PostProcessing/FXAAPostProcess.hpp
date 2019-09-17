#pragma once
#include "Util/Common.h"
#include "../Framebuffer.hpp"
#include "../Shader.hpp"
#include "PostProcess.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class MaterialComponent;
class FXAAPostProcess : public PostProcess {
public:
    FXAAPostProcess(Scene* scene);
    virtual void Run(Framebuffer* famebufferIn, Framebuffer* famebufferOut);

    Shader shader;
	Object3D* quad;
    MaterialComponent* material;
    Texture albedoTex;

    float minValue = 1.0 / 128.0;
    float maxSpan = 8.0;
    float reduceMultiplier = 1.0 / 8.0;
};
}
}