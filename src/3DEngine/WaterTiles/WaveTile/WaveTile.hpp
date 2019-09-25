#pragma once
#include "Util/Common.h"
#include "../../Object3D.hpp"
#include "../../Shader.hpp"
#include "../../Framebuffer.hpp"

namespace KikooRenderer {

namespace CoreEngine {

class MaterialComponent;
class MaterialComponent;

class WaveTile : public Object3D {
public: 
    WaveTile(std::string name, Scene* scene);

    void Start() override;
    void Enable() override;
    void Render(glm::mat4* overrideViewMatrix=nullptr) override;
    void DepthRenderPass(LightComponent* light) override;
    void Update() override;
    void Destroy() override;
    void Recompute() override;
    void WindowResize(int w, int h) override;

private:
    Object3D* quad;
	Object3D* dummyQuad;
    
    MaterialComponent* quadMaterial;
    Shader waterShader;

    Framebuffer* reflectionFramebuffer;
    Framebuffer* refractionFramebuffer;

    Texture dudvMap;
    Texture normalMap;

    float moveFactor=0;
    float speed = 0.03;
};

}
}