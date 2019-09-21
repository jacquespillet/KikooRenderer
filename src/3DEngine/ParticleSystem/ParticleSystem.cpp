#include "ParticleSystem.hpp"
#include "../Scene.hpp"
#include "../BaseObjects.hpp"
#include "../Texture.hpp"

#include "ParticleShader.hpp"
namespace KikooRenderer {

namespace CoreEngine {

void InsertionSort(std::vector<Particle>& arr)  
{  
    Particle key;
    int i, j;  
    for (i = 1; i < arr.size(); i++) 
    {  
        key = arr[i];  
        j = i - 1;  
  
        while (j >= 0 && arr[j].distance > key.distance) 
        {  
            arr[j + 1] = arr[j];  
            j = j - 1;  
        }  
        arr[j + 1] = key;  
    }  
} 

float GetRand() {
    return ((float)std::rand() / (float)RAND_MAX);
}

float GenerateValue(float average, float errorMargin) {
    float offset = (GetRand() - 0.5f) * 2.0f * errorMargin;
    return average + offset;
}

glm::vec3 GenerateRandomUnitVector() {
    float theta = (float) (GetRand() * 2.0f * PI);
    float z = (GetRand() * 2) - 1;
    float rootOneMinusZSquared = (float) std::sqrt(1 - z * z);
    float x = (float) (rootOneMinusZSquared * std::cos(theta));
    float y = (float) (rootOneMinusZSquared * std::sin(theta));
    return  glm::vec3(x, y, z);
}

glm::vec3 GenerateRandomUnitVectorWithinCone(glm::vec3 coneDirection, float angle) {
    float cosAngle = (float) std::cos(angle);
    float theta = (float) (GetRand() * 2.0f * PI);
    float z = cosAngle + (GetRand() * (1 - cosAngle));
    float rootOneMinusZSquared = (float) std::sqrt(1 - z * z);
    float x = (float) (rootOneMinusZSquared * std::cos(theta));
    float y = (float) (rootOneMinusZSquared * std::sin(theta));

    glm::vec4 direction(x, y, z, 1);
    if (coneDirection.x != 0 || coneDirection.y != 0 || (coneDirection.z != 1 && coneDirection.z != -1)) {
        glm::vec3 rotateAxis = glm::cross(coneDirection, glm::vec3(0, 0, 1));
        glm::normalize(rotateAxis);
        float rotateAngle = (float) std::acos(glm::dot(coneDirection, glm::vec3(0, 0, 1)));
        
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0), -rotateAngle, rotateAxis);
        direction = rotationMatrix * direction;
    } else if (coneDirection.z == -1) {
        direction.z *= -1;
    }
    return glm::vec3(direction);
}

ParticleSystem::ParticleSystem(std::string name, Scene* scene) : Object3D(name, scene) {
    particleShader = GetParticleShader();
    
    quad = GetQuad(scene, "particle", glm::dvec3(0), glm::dvec3(0), glm::dvec3(0.3), glm::dvec4(0.5, 0.5, 0.5, 1));
    quad->Start();
    quad->Enable();
    quad->faceCamera = true;

    quadMaterial = (MaterialComponent*) quad->GetComponent("Material");
    quadMaterial->SetShader(&particleShader);

    // Texture albedoTex = Texture("C:/Users/Jacques/Pictures/Textures/Particles/particleStar.png", GL_TEXTURE0);
    Texture albedoTex = Texture("C:/Users/Jacques/Pictures/Textures/Particles/particleAtlas.png", GL_TEXTURE0);
    quadMaterial->albedoTex = albedoTex;


    direction = glm::vec3(0, 1, 0);
}


void ParticleSystem::Start() {
	started = true;
}

void ParticleSystem::Enable() {
	enabled = true;
}

void ParticleSystem::Render() {
    GETGL
    ogl->glDepthMask(false);
    ogl->glBlendFunc(GL_SRC_ALPHA, GL_ONE);    

    for(int i=0; i<particles.size(); i++) {
        quad->transform->position = particles[i].position;
        quad->transform->scale = glm::vec3(particles[i].scale);
        quad->transform->rotation = glm::vec3(0, 0, particles[i].rotation);

        ogl->glUseProgram(particleShader.programShaderObject);

        ogl->glUniform2fv(ogl->glGetUniformLocation(particleShader.programShaderObject, "texOffset1"),1, glm::value_ptr(particles[i].offset1));
        ogl->glUniform2fv(ogl->glGetUniformLocation(particleShader.programShaderObject, "texOffset2"),1, glm::value_ptr(particles[i].offset2));
        ogl->glUniform2fv(ogl->glGetUniformLocation(particleShader.programShaderObject, "texCoordInfo"),1, glm::value_ptr(glm::vec2(particles[i].numRows, particles[i].blendValue)));

        quad->Render();
    }

    ogl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    
    ogl->glDepthMask(true);
}

void ParticleSystem::DepthRenderPass(LightComponent* light) {
}

void ParticleSystem::Update() {
    GenerateParticles();
    InsertionSort(particles);

    std::vector<Particle>::iterator it;
    for(it = particles.begin(); it != particles.end();)
    {
        bool alive = it->Update(scene->camera->transform->position);
        if(!alive) {
            it=particles.erase(it);
        } else ++it;
    }    
    scene->triggerRefresh = true;
}

void ParticleSystem::Destroy() {
    delete quad;
}

void ParticleSystem::Recompute() {
}

void ParticleSystem::GenerateParticles() {
    float delta = scene->deltaTime;
    if(delta * pps < 1.0) {
        toSpawn += delta * pps;
    } else {
        toSpawn = delta * pps;
    }    

    int spawnCount = (int) toSpawn;
    for(int i=0; i<spawnCount; i++) {
        EmitAdvancedParticle();
    }

    if (delta * pps < 1.0 && toSpawn >= 1.0)
    {
        toSpawn = 0.0f;
    }    
}

void ParticleSystem::EmitSimpleParticle() {
    glm::vec3 velocity = glm::vec3(GetRand() * 2.0 - 1.0, GetRand() * 2.0 - 1.0, GetRand() * 2.0 - 1.0);
    glm::normalize(velocity);
    velocity *= speed;
    Particle particle(scene, this->transform->position, velocity, gravityFactor, lifeLength, 0, 1);
    particles.push_back(particle);
}

void ParticleSystem::EmitAdvancedParticle() {
    glm::vec3 velocity(0);
    if(useDirection){
        velocity = GenerateRandomUnitVectorWithinCone(direction, directionDeviation);
    }else{
        velocity = GenerateRandomUnitVector();
    }
    glm::normalize(velocity);
    velocity *= GenerateValue(speed, speedError);

    float finalScale = GenerateValue(scale, scaleError);
    float finalLifeLength = GenerateValue(lifeLength, lifeError);
    float finalRotation = isRandomRotation ? GetRand() * 360.0f : 0;

    Particle particle(scene, this->transform->position, velocity, gravityFactor, finalLifeLength, finalRotation, finalScale);   
    particles.push_back(particle); 
}



}
}