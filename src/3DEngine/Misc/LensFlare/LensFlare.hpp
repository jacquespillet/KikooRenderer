#pragma once
#include "Util/Common.h"
#include "../../Object3D.hpp"
#include "../../Shader.hpp"
#include "../../Framebuffer.hpp"

namespace KikooRenderer {

namespace CoreEngine {

class MaterialComponent;
class MeshFilterComponent;
class CameraScene;

class LensFlare : public Object3D {
public: 
    LensFlare(std::string name, Scene* scene);

    void Start() override;
    void Enable() override;
    void LateRender() override;
    void Render(glm::mat4* overrideViewMatrix=nullptr) override;
    void DepthRenderPass(LightComponent* light) override;
    void Update() override;
    void Destroy() override;
    void Recompute() override;
    void WindowResize(int w, int h) override;
    std::vector<QWidget*> GetInspectorWidgets() override;


    QJsonObject ToJSON() {
        QJsonObject json;
        json["type"] = "LensFlare";
    }

private:
    Object3D* quad;
    
    MaterialComponent* quamaterial;
    MeshFilterComponent* mesh;
    Shader lensFlareShader;
    
    std::vector<Texture> textures;
    std::vector<glm::vec2> sizes;

    CameraScene* camera;

    float spacing;
    glm::vec2 screenCenter;
};

}
}