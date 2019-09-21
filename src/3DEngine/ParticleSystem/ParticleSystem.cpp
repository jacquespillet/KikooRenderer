#include "ParticleSystem.hpp"
#include "../Scene.hpp"
#include "../BaseObjects.hpp"
#include "../Texture.hpp"
#include "Util/RandomUtil.hpp"

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
  
        while (j >= 0 && arr[j].distance < key.distance) 
        {  
            arr[j + 1] = arr[j];  
            j = j - 1;  
        }  
        arr[j + 1] = key;  
    }  
} 


ParticleSystem::ParticleSystem(std::string name, Scene* scene) : Object3D(name, scene) {
    particleShader = GetParticleShader();
    
    quad = GetQuad(scene, "particle", glm::dvec3(0), glm::dvec3(0), glm::dvec3(0.3), glm::dvec4(0.5, 0.5, 0.5, 1));
    quad->Start();
    quad->Enable();
    quad->faceCamera = true;

    quadMaterial = (MaterialComponent*) quad->GetComponent("Material");
    quadMaterial->SetShader(&particleShader);

    Texture albedoTex = Texture("C:/Users/Jacques/Pictures/Textures/Particles/particleAtlas.png", GL_TEXTURE0);
    quadMaterial->albedoTex = albedoTex;
    numRows = 4;

    quadMeshFilter = (MeshFilterComponent*) quad->GetComponent("MeshFilter");
    quadMeshFilter->renderInstanced = true;

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
    ogl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    

    ogl->glUseProgram(particleShader.programShaderObject);
    ogl->glUniform1i(ogl->glGetUniformLocation(particleShader.programShaderObject, "rowNum"), numRows);
    
    quadMeshFilter->numInstances = particles.size();
    std::vector<MeshFilterComponent::InstanceAttribute> instanceAttributes = std::vector<MeshFilterComponent::InstanceAttribute>(particles.size());
    for(int i=0; i<particles.size(); i++) {
        quad->transform->position = particles[i].position;
        quad->transform->scale = glm::vec3(particles[i].scale);
        quad->transform->rotation = glm::vec3(0, 0, particles[i].rotation);

		glm::mat4 vMatrix = scene->camera->GetViewMatrix();
		glm::mat4 pMatrix = scene->camera->GetProjectionMatrix();
        glm::mat4 mMatrix = glm::translate(glm::mat4(1.0), glm::vec3(quad->transform->position));
        mMatrix[0][0] = vMatrix[0][0]; mMatrix[1][0] = vMatrix[0][1]; mMatrix[2][0] = vMatrix[0][2];
        mMatrix[0][1] = vMatrix[1][0]; mMatrix[1][1] = vMatrix[1][1]; mMatrix[2][1] = vMatrix[1][2];
        mMatrix[0][2] = vMatrix[2][0]; mMatrix[1][2] = vMatrix[2][1]; mMatrix[2][2] = vMatrix[2][2];
        mMatrix = glm::rotate(mMatrix, (float)quad->transform->rotation.z * (float)DEGTORAD, glm::vec3(0.0f, 0.0f, 1.0f));
        mMatrix = glm::scale(mMatrix, glm::vec3(quad->transform->scale));

        glm::mat4 mvp = pMatrix * vMatrix *  mMatrix;
        
        instanceAttributes[i].modelMatrix = mvp;
        instanceAttributes[i].additionalData1 = glm::vec4(particles[i].offset1.x, particles[i].offset1.y, particles[i].offset2.x, particles[i].offset2.y);
        instanceAttributes[i].additionalData2 = glm::vec4(particles[i].blendValue, 0, 0, 0);
    }

    quadMeshFilter->SetInstanceAttributes(instanceAttributes);
    quad->Render();

    ogl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    
    ogl->glDepthMask(true);
}

void ParticleSystem::DepthRenderPass(LightComponent* light) {
}

void ParticleSystem::Update() {
    GenerateParticles();

    std::vector<Particle>::iterator it;
    for(it = particles.begin(); it != particles.end();)
    {
        bool alive = it->Update(scene->camera->transform->position);
        if(!alive) {
            it=particles.erase(it);
        } else ++it;
    }    
    InsertionSort(particles);
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
        EmitParticle();
    }

    if (delta * pps < 1.0 && toSpawn >= 1.0)
    {
        toSpawn = 0.0f;
    }    
}


void ParticleSystem::EmitParticle() {
    glm::vec3 velocity(0);
    if(useDirection){
        velocity = Util::GenerateRandomUnitVectorWithinCone(direction, directionDeviation);
    }else{
        velocity = Util::GenerateRandomUnitVector();
    }
    glm::normalize(velocity);
    velocity *= Util::GenerateValue(speed, speedError);

    float finalScale = Util::GenerateValue(scale, scaleError);
    float finalLifeLength = Util::GenerateValue(lifeLength, lifeError);
    float finalRotation = isRandomRotation ? Util::GetRand() * 360.0f : 0;

    Particle particle(scene, numRows, this->transform->position, velocity, gravityFactor, finalLifeLength, finalRotation, finalScale);   
    particles.push_back(particle); 
}



}
}