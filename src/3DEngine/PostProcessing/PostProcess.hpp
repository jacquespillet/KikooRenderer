#pragma once
#include "Util/Common.h"
#include "../Framebuffer.hpp"
#include "../Shader.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class MaterialComponent;
class PostProcess {
public:
    PostProcess(Scene* scene);
    virtual void Run(Framebuffer* famebufferIn, Framebuffer* famebufferOut);

    Shader shader;
	Object3D* quad;
    MaterialComponent* material;
    Texture albedoTex;
};
}
}