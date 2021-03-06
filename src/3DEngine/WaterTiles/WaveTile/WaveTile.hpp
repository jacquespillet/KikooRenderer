#pragma once
#include "Util/Common.h"
#include "../../Object3D.hpp"
#include "../../Shader.hpp"
#include "../../Framebuffer.hpp"

namespace KikooRenderer {

namespace CoreEngine {

class MaterialComponent;
class MeshFilterComponent;

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
    std::vector<QWidget*> GetInspectorWidgets() override;


    QJsonObject ToJSON() {
        QJsonObject json;
        json["type"] = "WaveTile";
        json["numWaves"] = (int)(waves.size());

        QJsonArray wavesArray;
        for(int i=0; i<waves.size(); i++) {
            QJsonObject waveJson;
            waveJson["x"] = waves[i].x;
            waveJson["y"] = waves[i].y;
            waveJson["z"] = waves[i].z;
            waveJson["w"] = waves[i].w;
            wavesArray.append(waveJson);
        }
        json["waves"] = wavesArray;
    }

private:
    std::vector<glm::vec4> waves;
    Object3D* quad;
    
    MaterialComponent* quamaterial;
    MeshFilterComponent* mesh;
    Shader waterShader;
};

}
}