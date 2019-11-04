#pragma once
#include "Util/Common.h"
#include "../Object3D.hpp"
#include "../Shader.hpp"
#include "../Framebuffer.hpp"
#include "../Texture/Texture3D.hpp"

namespace KikooRenderer {

namespace CoreEngine {

class MaterialComponent;
class MeshFilterComponent;

class Cloud_1 : public Object3D {
public: 
    Cloud_1(std::string name, Scene* scene);

    void Start() override;
    void Enable() override;
    void Render(glm::mat4* overrideViewMatrix=nullptr) override;
    void RayMarch(Framebuffer* fb) override;
    void DepthRenderPass(LightComponent* light) override;
    void Update() override;
    void Destroy() override;
    void Recompute() override;
    void WindowResize(int w, int h) override;
    std::vector<QWidget*> GetInspectorWidgets() override;


    QJsonObject ToJSON() {
        QJsonObject json;
        json["type"] = "Cloud_1";
    }

private:

    Shader cloudShader;
    Object3D* quad;
    Framebuffer* fb;
    Texture3D noiseTex;
    
    MaterialComponent* quadMaterial;
    MeshFilterComponent* mesh;
};

}
}