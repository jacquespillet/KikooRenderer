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
    void DepthRenderPass(LightComponent* light) override;
    void Update() override;
    void Destroy() override;
    void Recompute() override;

    void GenerateParticles();
    void EmitParticle();

private:
    std::vector<Particle> particles;
    Object3D* quad;
    MaterialComponent* quadMaterial;
    MeshFilterComponent* quadMeshFilter;
    Shader particleShader;

    //_________________________
    // Main parameters
    float pps = 200;
    float speed = 2;
    float scale = 0.5;
    float lifeLength = 4;
    float gravityFactor = 0.01;
    
    //_________________________
    // Randomization parameters
    float speedError = 0.3;
    float lifeError = 0.5;
    float scaleError = 0.5;
    bool isRandomRotation = true;

    //_________________________
    // System direction parameters
    glm::vec3 direction;
    bool useDirection = true;
    float directionDeviation = 0.4;

    //_________________________
    // Blend mode : 
    //  * 0 : additive
    //  * 1 : One minus src alpha
    int blendingMode = 0;
    
    float toSpawn = 0;

    int numRows = 1;
};

}
}