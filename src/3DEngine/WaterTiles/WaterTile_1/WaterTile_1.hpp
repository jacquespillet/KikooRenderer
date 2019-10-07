#pragma once
#include "Util/Common.h"
#include "../../Object3D.hpp"
#include "../../Shader.hpp"
#include "../../Framebuffer.hpp"

namespace KikooRenderer {

namespace CoreEngine {

class MaterialComponent;
class MaterialComponent;

class WaterTile_1 : public Object3D {
public: 
    WaterTile_1(std::string name, Scene* scene);

    void Start() override;
    void Enable() override;
    void Render(glm::mat4* overrideViewMatrix=nullptr) override;
    void DepthRenderPass(LightComponent* light) override;
    void Update() override;
    void Destroy() override;
    void Recompute() override;
    void WindowResize(int w, int h) override;
    std::vector<QWidget*> GetInspectorWidgets() override;


private:
    Object3D* quad;
	Object3D* dummyQuad;
    
    MaterialComponent* quamaterial;
    Shader waterShader;

    Framebuffer* reflectionFramebuffer;
    Framebuffer* refractionFramebuffer;

    Texture dudvMap;
    Texture normalMap;

    float moveFactor=0;
    float waveStrength = 0.02;
    int reflectivity = 10;
    float speed = 0.03;
    float blueness = 0.3;


    QJsonObject ToJSON() {
        QJsonObject json;
        json["type"] = "BoundingBox";

        QJsonObject minJson;
        minJson["x"] = min.x;
        minJson["y"] = min.y;
        minJson["z"] = min.z;
        json["min"] = minJson;

        QJsonObject maxJson;
        maxJson["x"] = max.x;
        maxJson["y"] = max.y;
        maxJson["z"] = max.z;
        json["max"] = maxJson;
        
        return json;        
    }    
};

}
}