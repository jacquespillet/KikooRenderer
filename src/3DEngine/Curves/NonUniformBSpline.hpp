#pragma once
#include "Util/Common.h"
#include "../Object3D.hpp"
#include "../Shader.hpp"
#include "../Framebuffer.hpp"

namespace KikooRenderer {

namespace CoreEngine {

class MaterialComponent;
class MeshFilterComponent;

class NonUniformBSpline : public Object3D {
public: 
    NonUniformBSpline(std::string name, Scene* scene);

    void Start() override;
    void Enable() override;
    void Render(glm::mat4* overrideViewMatrix=nullptr) override;
    void DepthRenderPass(LightComponent* light) override;
    void Update() override;
    void Destroy() override;
    void Recompute() override;
    void WindowResize(int w, int h) override;
    
    void ComputePositions();

    std::vector<QWidget*> GetInspectorWidgets() override;


    QJsonObject ToJSON() {
        QJsonObject json;
        json["type"] = "NonUniformBSpline";
        // json["numWaves"] = (int)(waves.size());

        // QJsonArray wavesArray;
        // for(int i=0; i<waves.size(); i++) {
        //     QJsonObject waveJson;
        //     waveJson["x"] = waves[i].x;
        //     waveJson["y"] = waves[i].y;
        //     waveJson["z"] = waves[i].z;
        //     waveJson["w"] = waves[i].w;
        //     wavesArray.append(waveJson);
        // }
        // json["waves"] = wavesArray;
    }

protected:
    double CoxDeBoor(double i, double k, double u, std::vector<double> knotVector);

    Object3D* line;
    MaterialComponent* material;
    MeshFilterComponent* mesh;

    double offset = 0.01;
    
    std::vector<glm::vec3> vertex;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec4> colors;
    std::vector<int> triangles;

    std::vector<glm::vec3> points;
    std::vector<double> knotVector;
};

}
}