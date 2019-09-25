#pragma once
#include "Util/Common.h"
#include "../../Object3D.hpp"
#include "../../Shader.hpp"
#include "../../Framebuffer.hpp"

namespace KikooRenderer {

namespace CoreEngine {

class MaterialComponent;

enum TILE_TYPE {WATER, DIRECTIONAL};
class WaterTile_2 : public Object3D {
public: 

    WaterTile_2(std::string name, Scene* scene);

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
    MaterialComponent* quadMaterial;
    Shader waterShader;

    Texture colorTexture;    
    Texture flowMap;    
    Texture normalMap;
    TILE_TYPE tileType = TILE_TYPE::DIRECTIONAL;
};

}
}