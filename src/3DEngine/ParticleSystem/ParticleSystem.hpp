#pragma once
#include "Util/Common.h"
#include "../Object3D.hpp"
#include "../Shader.hpp"

#include "Particle.hpp"

namespace KikooRenderer {

namespace CoreEngine {
class Scene;
class MaterialComponent;
class MeshFilterComponent;

class ParticleSystem : public Object3D {
public: 
    ParticleSystem(std::string name, Scene* scene);

    void Start() override;
    void Enable() override;
    void Render(glm::mat4* overrideViewMatrix = nullptr) override;
    void LateRender() override;
    void DepthRenderPass(LightComponent* light) override;
    void Update() override;
    void Destroy() override;
    void Recompute() override;
    std::vector<QWidget*> GetInspectorWidgets() override;

    void GenerateParticles();
    void EmitParticle();

    QWidget* GetMainParameters();


    
    float GetAmplitude();
    void SetAmplitude(float _amplitude);
    
    float GetFrequencyX();
    void SetFrequencyX(float _frequencyX);
    
    float GetFrequencyY();
    void SetFrequencyY(float _frequencyY);
    
    float GetFrequencyZ();
    void SetFrequencyZ(float _frequencyZ);
    
    int GetOctavesX();
    void SetOctavesX(int _octavesX);
    
    int GetOctavesY();
    void SetOctavesY(int _octavesY);
    
    int GetOctavesZ();
    void SetOctavesZ(int _octavesZ);
    
    float GetOffsetMult();
    void SetOffsetMult(float _offsetMult);
    
    float GetSpeedMult();
    void SetSpeedMult(float _speedMult);



    QJsonObject ToJSON() {
        QJsonObject json;
        json["type"] = "ParticleSystem";
        json["pps"] = pps;
        json["speed"] = speed;
        json["scale"] = scale;
        json["lifeLength"] = lifeLength;
        json["gravityFactor"] = gravityFactor;
        json["speedError"] = speedError;
        json["lifeError"] = lifeError;
        json["scaleError"] = scaleError;
        json["isRandomRotation"] = isRandomRotation;
        json["useDirection"] = useDirection;
        json["directionDeviation"] = directionDeviation;
        json["blendingMode"] = blendingMode;
        json["numRows"] = numRows;
        json["useCurlNoise"] = useCurlNoise;
        json["textureFile"] = QString::fromStdString(textureFile);
        return json;        
    }       

private:
    std::vector<Particle> particles;
    Object3D* quad;
    MaterialComponent* quadmaterial;
    MeshFilterComponent* quadMeshFilter;
    Shader particleShader;

    std::string textureFile ="resources/Textures/Particles/particleAtlas.png";

    //_________________________
    // Main parameters
    float pps = 200;
    float speed = 5;
    float scale = 1;
    float lifeLength = 10;
    float gravityFactor = 0.2;
    
    //_________________________
    // Randomization parameters
    float speedError = 0.3;
    float lifeError = 0.5;
    float scaleError = 0.2;
    bool isRandomRotation = true;

    //_________________________
    // System direction parameters
    bool useDirection = true;
    float directionDeviation = 0.4;

    //_________________________
    // Blend mode : 
    //  * 0 : additive
    //  * 1 : One minus src alpha
    int blendingMode = 0;
    
    float toSpawn = 0;

    int numRows = 1;

    //Curl noise params
    bool useCurlNoise = false;
        
    float amplitude = 1;
    float frequencyX = 0.05;
    float frequencyY = 0.05;
    float frequencyZ = 0.05;
    int octavesX = 4;
    int octavesY = 4;
    int octavesZ = 4;
    float offsetMult = 0.3;
    float speedMult = 4.0;
};

}
}